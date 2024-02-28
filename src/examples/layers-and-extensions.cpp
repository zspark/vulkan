#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <vulkan/vulkan.h>

#include "vulkan-tutorial/printer.hpp"
#include "vulkan-tutorial/vulkan-utils.hpp"

const char *const APP_NAME = "layers and extensions";
const char *const ENGINE_NAME = "ratel";

static void _printLayers() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> _availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, _availableLayers.data());

    printer::layers(_availableLayers);
}

static void _printInstanceExtensions() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> _extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, _extensions.data());

    printer::extensions(_extensions, "instance");
}

static void _printDeviceExtensions(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    printer::extensions(availableExtensions, "physical device");
}

int main(int argc, char **argv) {
    _printLayers();
    _printInstanceExtensions();

    VkInstance _ins{VK_NULL_HANDLE};
    VkPhysicalDevice _phyDev{VK_NULL_HANDLE};

    if (!(_ins = vulkan::createInstance(APP_NAME, ENGINE_NAME))) {
        goto destroy;
    }

    if (!(_phyDev = vulkan::getPhysicalDevice(_ins))) {
        goto destroy;
    }
    _printDeviceExtensions(_phyDev);

destroy:
    fprintf(stdout, "destroy called.");
    if (_ins)
        vkDestroyInstance(_ins, nullptr);
    exit(EXIT_SUCCESS);
}
