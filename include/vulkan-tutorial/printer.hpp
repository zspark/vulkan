#ifndef _HEADER_PRINTER_
#define _HEADER_PRINTER_

#include <stdio.h>
#include <vector>

#include <vulkan/vulkan.h>

namespace printer{
    static void layers( const std::vector<VkLayerProperties>& arr){
        fprintf(stdout, "available layers:(%d)\n",static_cast<uint32_t>(arr.size()));
        for (const auto& layerProperties : arr) {
        fprintf(stdout, "\tname:%s, specVersion:%d, implementationVersion:%d, description:%s\n", layerProperties.layerName,
                layerProperties.specVersion,
                layerProperties.implementationVersion, layerProperties.description);
        }
    }

    static void extensions( const std::vector<VkExtensionProperties>& arr){
      fprintf(stdout, "available extensions:(%d)\n",static_cast<uint32_t>(arr.size()));
      for (const auto &extension : arr) {
            fprintf(stdout, "\tname:%s, specVersion:%d\n", extension.extensionName, extension.specVersion);
        }
    }

    //static void physicalDevice(VkPhysicalDeviceProperties properties, VkPhysicalDeviceFeatures features){
    static void physicalDevice(VkPhysicalDevice device){
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &properties);
        //vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        fprintf(stdout,
                "\tdevice properties:\n"
                "\t\tdeviceName:%s, apiVersion:%d, driverVersion:%d, "
                "vendorID:%d, deviceID:%d, deviceType:%d\n",
                properties.deviceName, properties.apiVersion,
                properties.driverVersion, properties.vendorID,
                properties.deviceID, properties.deviceType);
    }

    static void queueFamilies(const std::vector<VkQueueFamilyProperties>& arr){
      fprintf(stdout,
              "VK_QUEUE_GRAPHICS_BIT(%x)\nVK_QUEUE_COMPUTE_BIT(%x)\nVK_QUEUE_"
              "TRANSFER_BIT(%x)\nVK_QUEUE_SPARSE_BINDING_BIT(%x)\nVK_QUEUE_"
              "PROTECTED_BIT(%x)\n",
              VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_COMPUTE_BIT,
              VK_QUEUE_TRANSFER_BIT, VK_QUEUE_SPARSE_BINDING_BIT,
              VK_QUEUE_PROTECTED_BIT);
        fprintf(stdout,"queue family:(%d)\n", static_cast<uint32_t>(arr.size()));
        for (const auto &queueFamily : arr) {
            fprintf(stdout, "\tqueueFlags:%x, queueCount:%d, timestampValidBits:%d\n",
                queueFamily.queueFlags, queueFamily.queueCount, queueFamily.timestampValidBits);
        }
    }
}


#endif
