#ifndef _HEADER_VULKAN_UTILS_
#define _HEADER_VULKAN_UTILS_

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdio.h>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.h>

#include "vulkan-tutorial/printer.hpp"

const std::vector<const char *> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};
const std::vector<const char *> DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

namespace vulkan {
#ifndef NDEBUG
// typedef VkResult createDebugUtilsMessengerFn(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT *, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pMessenger);
// typedef void destroyDebugUtilsMessengerFn(VkInstance, VkDebugUtilsMessengerEXT);
static PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
static PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
#endif

static VkInstance createInstance(const char *appName, const char *engineName) {
#ifndef NDEBUG
    auto _checkValidationLayerSupport = []() -> bool {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName : VALIDATION_LAYERS) {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    };

    if (!_checkValidationLayerSupport()) {
        fprintf(stderr, "validation layers requested, but not available!\n");
        return VK_NULL_HANDLE;
    }
#endif

    auto _getRequiredExtensions = []() -> std::vector<const char *> {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        fprintf(stdout, "glfwExtensionCount:%d\n", glfwExtensionCount);
        for (int i = 0; i < glfwExtensionCount; ++i) {
            fprintf(stdout, "glfwExtensions[%d]: %s\n", i, glfwExtensions[i]);
        }

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
#ifndef NDEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
        // extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        return extensions;
    };

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char *> _vecExtension = _getRequiredExtensions();
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(_vecExtension.size());
    createInfo.ppEnabledExtensionNames = _vecExtension.data();

    createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

    VkInstance _ins;
    if (vkCreateInstance(&createInfo, nullptr, &_ins) != VK_SUCCESS) {
        fprintf(stderr, "failed to create instance!\n");
        return VK_NULL_HANDLE;
    }

#ifndef NDEBUG
    vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_ins, "vkCreateDebugUtilsMessengerEXT");
    vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_ins, "vkDestroyDebugUtilsMessengerEXT");
#endif
    return _ins;
}

#ifndef NDEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL _debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE; // VK_FALSE means the Vulkan call that triggered this callback should not be aborted
}
static VkDebugUtilsMessengerEXT createDebugUtilsMessenger(VkInstance ins) {
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = _debugCallback;

    VkDebugUtilsMessengerEXT debugMessenger;
    if (VK_SUCCESS != vkCreateDebugUtilsMessengerEXT(ins, &debugCreateInfo, nullptr, &debugMessenger)) {
        fprintf(stderr, "failed to create debug utils messenger\n");
        return VK_NULL_HANDLE;
    }

    fprintf(stdout, "create debug utils messenger successfully.\n");
    return debugMessenger;
}
static void destroyDebugUtilsMessenger(VkInstance ins, VkDebugUtilsMessengerEXT msg) { vkDestroyDebugUtilsMessengerEXT(ins, msg, nullptr); }
#endif

static VkPhysicalDevice getPhysicalDevice(VkInstance ins) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(ins, &deviceCount, nullptr);
    if (deviceCount == 0) {
        fprintf(stderr, "failed to find GPUs with Vulkan support!\n");
        return nullptr;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(ins, &deviceCount, devices.data());

    const auto _isDeviceSuitable = [&](VkPhysicalDevice device) {
        printer::physicalDevice(device);

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
    };

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    fprintf(stdout, "device count(%d)\n", deviceCount);
    for (const auto &device : devices) {
        if (_isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE) {
        fprintf(stdout, "failed to find a suitable GPU!\n");
        return nullptr;
    }

    return physicalDevice;
}

static VkSurfaceKHR createSurface(VkInstance ins, GLFWwindow *window) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(ins, window, nullptr, &surface) != VK_SUCCESS) {
        fprintf(stderr, "[GLFW] failed to create window surface!\n");
        return nullptr;
    }

    fprintf(stdout, "window surface created.\n");
    return surface;
}

/**
 * graphicsFamily:
 * The graphicsFamily typically represents the queue family that is responsible for handling graphics commands and rendering tasks in Vulkan.
 * Commands related to graphics processing, such as drawing and rendering, are submitted to a queue associated with the graphics family.
 *
 * presentFamily:
 * The presentFamily refers to the queue family that is responsible for presenting the rendered images to the screen or the window surface.
 * In Vulkan, after rendering a frame, the images need to be presented to the display. The presentFamily is associated with the queue that handles this presentation process.
 */
struct CapableQueueFamilies {
    std::optional<uint32_t> graphicsFamilyIndex = std::nullopt;
    std::optional<uint32_t> presentFamilyIndex = std::nullopt;

    bool isComplete() const { return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value(); }
};

static bool findCapableQueueFamilies(const VkSurfaceKHR surface, const VkPhysicalDevice phyDev, CapableQueueFamilies *indices) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(phyDev, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(phyDev, &queueFamilyCount, queueFamilies.data());
    printer::queueFamilies(queueFamilies);

    uint32_t i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices->graphicsFamilyIndex = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(phyDev, i, surface, &presentSupport);
        if (presentSupport) {
            indices->presentFamilyIndex = i;
        }

        if (indices->isComplete()) {
            fprintf(stdout, "all queue families are found!\n");
            return true;
            break;
        }

        ++i;
    }
    return false;
}

/**
 * different queue create infos with exact same parameter will result in fake dead.
 * so, here we must use `unordered_set` OR `set` as well.
 */
static VkDevice createLogicalDevice(VkPhysicalDevice phyDev, const std::unordered_set<uint32_t> &&vecIndex) {
    float queuePriority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> _vecQueueCreateInfo{};
    for (const auto &index : vecIndex) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = index;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfo.pNext = NULL;
        queueCreateInfo.flags = 0;
        _vecQueueCreateInfo.emplace_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(phyDev, &deviceFeatures);

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.pQueueCreateInfos = _vecQueueCreateInfo.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(_vecQueueCreateInfo.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

#ifndef NDEBUG
    createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    VkDevice _dev;
    VkResult _res = vkCreateDevice(phyDev, &createInfo, nullptr, &_dev);
    if (_res != VK_SUCCESS) {
        fprintf(stderr, "failed to create logical device!\n");
        fprintf(stderr, "vulkan API error: %x(HEX) %d(Dec)", _res, _res);
        return nullptr;
    } else {
    }

    fprintf(stdout, "logical device create complete\n");
    return _dev;
}

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

static VkSwapchainKHR createSwapChain(GLFWwindow *window, VkPhysicalDevice phyDev, VkDevice dev, VkSurfaceKHR surface, const CapableQueueFamilies *pIndices, VkFormat *pFmt = nullptr, VkExtent2D *pExt = nullptr) {
    auto _querySwapChainSupport = [](VkPhysicalDevice phyDev, VkSurfaceKHR surface) -> SwapChainSupportDetails {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phyDev, surface, &details.capabilities);
        printer::surfaceCapabilities(details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(phyDev, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(phyDev, surface, &formatCount, details.formats.data());
            printer::surfaceFormats(details.formats);
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(phyDev, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(phyDev, surface, &presentModeCount, details.presentModes.data());
            printer::surfacePresentModes(details.presentModes);
        }

        return details;
    };

    auto _chooseSwapSurfaceFormat = [](const std::vector<VkSurfaceFormatKHR> &availableFormats) -> VkSurfaceFormatKHR {
        for (const auto &availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    };

    auto _chooseSwapPresentMode = [](const std::vector<VkPresentModeKHR> &availablePresentModes) -> VkPresentModeKHR {
        for (const auto &availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    };

    auto _chooseSwapExtent = [window](const VkSurfaceCapabilitiesKHR &capabilities) -> VkExtent2D {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    };

    SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(phyDev, surface);
    VkSurfaceFormatKHR surfaceFormat = _chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = _chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = _chooseSwapExtent(swapChainSupport.capabilities);

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    /**
     * However, simply sticking to this minimum means that we may sometimes have to wait on the driver to complete internal operations before we can acquire another image to render to.
     * Therefore it is recommended to request at least one more image than the minimum:
     * We should also make sure to not exceed the maximum number of images while doing this, where 0 is a special value that means that there is no maximum:
     */
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    createInfo.minImageCount = imageCount;

    uint32_t queueFamilyIndices[] = {pIndices->graphicsFamilyIndex.value(), pIndices->presentFamilyIndex.value()};

    if (pIndices->graphicsFamilyIndex != pIndices->presentFamilyIndex) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VkSwapchainKHR _swapChain;
    if (vkCreateSwapchainKHR(dev, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
        fprintf(stderr, "failed to create swap chain!\n");
        return nullptr;
    }
    *pFmt = surfaceFormat.format;
    pExt->width = extent.width;
    pExt->height = extent.height;
    fprintf(stdout, "swap chain created!\n");
    return _swapChain;
}

static bool getSwapchainImages(VkDevice dev, VkSwapchainKHR swapChain, std::vector<VkImage> &out) {
    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(dev, swapChain, &imageCount, nullptr);
    out.resize(imageCount);
    vkGetSwapchainImagesKHR(dev, swapChain, &imageCount, out.data());
    return true;
}

static bool createImageViews(VkDevice dev, const std::vector<VkImage> &vecImage, VkFormat format, VkExtent2D ext, std::vector<VkImageView> &out) {
    out.resize(vecImage.size());

    for (size_t i = 0; i < vecImage.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = vecImage[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(dev, &createInfo, nullptr, &out[i]) != VK_SUCCESS) {
            fprintf(stderr, "failed to create image views!\n");
            return false;
        }
    }
    fprintf(stdout, "image views created!\n");
    return true;
}
} // namespace vulkan

#endif
