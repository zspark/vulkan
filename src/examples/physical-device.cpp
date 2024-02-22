#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "vulkan-tutorial/printer.hpp"

const char* const APP_NAME="physical device";
const char* const ENGINE_NAME="ratel";
 
static void _errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}
 
static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    fprintf(stdout, "Key pressed, key:%d, scancode:%d, action:%d, mods:%d\n", key,scancode,action,mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static GLFWwindow* _createGLFWwindow(const char const *caption, int w = 640, int h = 480) {
    GLFWwindow* out = glfwCreateWindow(w, h, caption, NULL, NULL);
    if (!out) {
        fprintf(stderr, "failed to create glfw window!\n");
        return nullptr;
    }
 
    glfwSetKeyCallback(out, _keyCallback);
    glfwSetErrorCallback(_errorCallback);
    glfwMakeContextCurrent(out);
    glfwSwapInterval(1);

    return out;
}

static VkInstance _createInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = APP_NAME;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = ENGINE_NAME;
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    fprintf(stdout,"glfwExtensionCount:%d\n",glfwExtensionCount);
    for(int i=0;i<glfwExtensionCount;++i){
        fprintf(stdout,"glfwExtensions[%d]: %s\n",i,glfwExtensions[i]);
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    VkInstance _ins;
    if (vkCreateInstance(&createInfo, nullptr, &_ins) != VK_SUCCESS) {
        fprintf(stderr, "failed to create instance!\n");
        return nullptr;
    }
    return _ins;
}

static VkPhysicalDevice _createPhysicalDevice(VkInstance ins){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(ins, &deviceCount, nullptr);
    if (deviceCount == 0) {
        fprintf(stderr, "failed to find GPUs with Vulkan support!\n");
        return nullptr;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(ins, &deviceCount, devices.data());

    const auto _isDeviceSuitable=[&](VkPhysicalDevice device) {
        printer::physicalDevice(device);

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
    };

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    fprintf(stdout,"device count(%d)\n",deviceCount);
    for (const auto& device : devices) {
        if (_isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE) {
        fprintf(stdout,"failed to find a suitable GPU!\n");
        return nullptr;
    }

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    printer::queueFamilies(queueFamilies);

    return physicalDevice;
}

int main(int argc, char** argv) {
    if (!glfwInit()) exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* _wnd = _createGLFWwindow(APP_NAME);
    if(!_wnd){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    VkInstance _ins = _createInstance();
    if(!_ins){
        glfwDestroyWindow(_wnd);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    VkPhysicalDevice _dev = _createPhysicalDevice(_ins);
    if(!_dev){
        vkDestroyInstance(_ins, nullptr);
        glfwDestroyWindow(_wnd);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    while (!glfwWindowShouldClose(_wnd)) {
        glfwSwapBuffers(_wnd);
        glfwPollEvents();
    }

    vkDestroyInstance(_ins, nullptr);
    glfwDestroyWindow(_wnd);
    glfwTerminate();
    exit(EXIT_SUCCESS);

}


