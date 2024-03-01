#ifndef _HEADER_PRINTER_
#define _HEADER_PRINTER_

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

std::string printVkData(const VkExtent2D &v) {
    std::stringstream ss;
    ss << "(" << v.width << " , " << v.height << ")";
    return ss.str();
}
std::string printVkData(const VkExtent3D &v) {
    std::stringstream ss;
    ss << "(" << v.width << " , " << v.height << " , " << v.depth << ")";
    return ss.str();
}

namespace printer {
using namespace std;
static void layers(const vector<VkLayerProperties> &arr, const char *desc) {
    fprintf(stdout, "%s layers count: %d\n", desc, static_cast<uint32_t>(arr.size()));
    for (const auto &layerProperties : arr) {
        fprintf(stdout, "\tname:%s, specVersion:%d, implementationVersion:%d, description:%s\n", layerProperties.layerName, layerProperties.specVersion, layerProperties.implementationVersion, layerProperties.description);
    }
}

static void extensions(const vector<VkExtensionProperties> &arr, const char *desc) {
    fprintf(stdout, "%s extensions count: %d\n", desc, static_cast<uint32_t>(arr.size()));
    for (const auto &extension : arr) {
        fprintf(stdout, "\tname:%s, specVersion: %d\n", extension.extensionName, extension.specVersion);
    }
}

static void physicalDevice(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    cout << "\tdevice properties:" << endl                            // a
         << "\t\tdeviceName: " << properties.deviceName << endl       // a
         << "\t\tapiVersion: " << properties.apiVersion << endl       // a
         << "\t\tdriverVersion: " << properties.driverVersion << endl // a
         << "\t\tvendorID: " << properties.vendorID << endl           // a
         << "\t\tdeviceID: " << properties.deviceID << endl           // a
         << "\t\tdeviceType: " << properties.deviceType << endl;      // a

    cout << "\tdevice features:" << endl
         << "\t\trobustBufferAccess: " << deviceFeatures.robustBufferAccess << endl                                           // a
         << "\t\tfullDrawIndexUint32: " << deviceFeatures.fullDrawIndexUint32 << endl                                         // a
         << "\t\timageCubeArray: " << deviceFeatures.imageCubeArray << endl                                                   // a
         << "\t\tindependentBlend: " << deviceFeatures.independentBlend << endl                                               // a
         << "\t\tgeometryShader: " << deviceFeatures.geometryShader << endl                                                   // a
         << "\t\ttessellationShader: " << deviceFeatures.tessellationShader << endl                                           // a
         << "\t\tsampleRateShading: " << deviceFeatures.sampleRateShading << endl                                             // a
         << "\t\tdualSrcBlend: " << deviceFeatures.dualSrcBlend << endl                                                       // a
         << "\t\tlogicOp: " << deviceFeatures.logicOp << endl                                                                 // a
         << "\t\tmultiDrawIndirect: " << deviceFeatures.multiDrawIndirect << endl                                             // a
         << "\t\tdrawIndirectFirstInstance: " << deviceFeatures.drawIndirectFirstInstance << endl                             // a
         << "\t\tdepthClamp: " << deviceFeatures.depthClamp << endl                                                           // a
         << "\t\tdepthBiasClamp: " << deviceFeatures.depthBiasClamp << endl                                                   // a
         << "\t\tfillModeNonSolid: " << deviceFeatures.fillModeNonSolid << endl                                               // a
         << "\t\tdepthBounds: " << deviceFeatures.depthBounds << endl                                                         // a
         << "\t\twideLines: " << deviceFeatures.wideLines << endl                                                             // a
         << "\t\tlargePoints: " << deviceFeatures.largePoints << endl                                                         // a
         << "\t\talphaToOne: " << deviceFeatures.alphaToOne << endl                                                           // a
         << "\t\tmultiViewport: " << deviceFeatures.multiViewport << endl                                                     // a
         << "\t\tsamplerAnisotropy: " << deviceFeatures.samplerAnisotropy << endl                                             // a
         << "\t\ttextureCompressionETC2: " << deviceFeatures.textureCompressionETC2 << endl                                   // a
         << "\t\ttextureCompressionASTC_LDR: " << deviceFeatures.textureCompressionASTC_LDR << endl                           // a
         << "\t\ttextureCompressionBC: " << deviceFeatures.textureCompressionBC << endl                                       // a
         << "\t\tocclusionQueryPrecise: " << deviceFeatures.occlusionQueryPrecise << endl                                     // a
         << "\t\tpipelineStatisticsQuery: " << deviceFeatures.pipelineStatisticsQuery << endl                                 // a
         << "\t\tvertexPipelineStoresAndAtomics: " << deviceFeatures.vertexPipelineStoresAndAtomics << endl                   // a
         << "\t\tfragmentStoresAndAtomics: " << deviceFeatures.fragmentStoresAndAtomics << endl                               // a
         << "\t\tshaderTessellationAndGeometryPointSize: " << deviceFeatures.shaderTessellationAndGeometryPointSize << endl   // a
         << "\t\tshaderImageGatherExtended: " << deviceFeatures.shaderImageGatherExtended << endl                             // a
         << "\t\tshaderStorageImageMultisample: " << deviceFeatures.shaderStorageImageMultisample << endl                     // a
         << "\t\tshaderStorageImageReadWithoutFormat: " << deviceFeatures.shaderStorageImageReadWithoutFormat << endl         // a
         << "\t\tshaderStorageImageWriteWithoutFormat: " << deviceFeatures.shaderStorageImageWriteWithoutFormat << endl       // a
         << "\t\tshaderUniformBufferArrayDynamicIndexing: " << deviceFeatures.shaderUniformBufferArrayDynamicIndexing << endl // a
         << "\t\tshaderSampledImageArrayDynamicIndexing: " << deviceFeatures.shaderSampledImageArrayDynamicIndexing << endl   // a
         << "\t\tshaderStorageBufferArrayDynamicIndexing: " << deviceFeatures.shaderStorageBufferArrayDynamicIndexing << endl // a
         << "\t\tshaderStorageImageArrayDynamicIndexing: " << deviceFeatures.shaderStorageImageArrayDynamicIndexing << endl   // a
         << "\t\tshaderClipDistance: " << deviceFeatures.shaderClipDistance << endl                                           // a
         << "\t\tshaderCullDistance: " << deviceFeatures.shaderCullDistance << endl                                           // a
         << "\t\tshaderFloat64: " << deviceFeatures.shaderFloat64 << endl                                                     // a
         << "\t\tshaderInt64: " << deviceFeatures.shaderInt64 << endl                                                         // a
         << "\t\tshaderInt16: " << deviceFeatures.shaderInt16 << endl                                                         // a
         << "\t\tshaderResourceResidency: " << deviceFeatures.shaderResourceResidency << endl                                 // a
         << "\t\tshaderResourceMinLod: " << deviceFeatures.shaderResourceMinLod << endl                                       // a
         << "\t\tsparseBinding: " << deviceFeatures.sparseBinding << endl                                                     // a
         << "\t\tsparseResidencyBuffer: " << deviceFeatures.sparseResidencyBuffer << endl                                     // a
         << "\t\tsparseResidencyImage2D: " << deviceFeatures.sparseResidencyImage2D << endl                                   // a
         << "\t\tsparseResidencyImage3D: " << deviceFeatures.sparseResidencyImage3D << endl                                   // a
         << "\t\tsparseResidency2Samples: " << deviceFeatures.sparseResidency2Samples << endl                                 // a
         << "\t\tsparseResidency4Samples: " << deviceFeatures.sparseResidency4Samples << endl                                 // a
         << "\t\tsparseResidency8Samples: " << deviceFeatures.sparseResidency8Samples << endl                                 // a
         << "\t\tsparseResidency16Samples: " << deviceFeatures.sparseResidency16Samples << endl                               // a
         << "\t\tsparseResidencyAliased: " << deviceFeatures.sparseResidencyAliased << endl                                   // a
         << "\t\tvariableMultisampleRate: " << deviceFeatures.variableMultisampleRate << endl                                 // a
         << "\t\tinheritedQueries: " << deviceFeatures.inheritedQueries << endl;                                              // a
}

static void queueFamilies(const vector<VkQueueFamilyProperties> &arr) {
    fprintf(stdout,
            "VK_QUEUE_GRAPHICS_BIT = %x\nVK_QUEUE_COMPUTE_BIT = %x\nVK_QUEUE_"
            "TRANSFER_BIT = %x\nVK_QUEUE_SPARSE_BINDING_BIT = %x\nVK_QUEUE_"
            "PROTECTED_BIT = %x\n",
            VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_SPARSE_BINDING_BIT, VK_QUEUE_PROTECTED_BIT);

    fprintf(stdout, "queue families count: %d, (QUEUE FAMILY IS A ORDERED LIST.)\n", static_cast<uint32_t>(arr.size()));
    for (const auto &queueFamily : arr) {
        fprintf(stdout, "\tqueueFlags:%x, queueCount:%d, timestampValidBits:%d\n", queueFamily.queueFlags, queueFamily.queueCount, queueFamily.timestampValidBits);
    }
}

static void surfaceCapabilities(const VkSurfaceCapabilitiesKHR &value) {
    fprintf(stdout, "surface capabilities.\n");
    // 0 is a special value that means that there is no maximum:
    cout                                                                                                  // a
        << left << setw(30) << "\tminImageCount: " << right << value.minImageCount << endl                // sfsf
        << left << setw(30) << "\tmaxImageCount: " << right << value.maxImageCount << endl                // sfsf
        << left << setw(30) << "\tcurrentExtent: " << right << printVkData(value.currentExtent) << endl   // sfsf
        << left << setw(30) << "\tminImageExtent: " << right << printVkData(value.minImageExtent) << endl // sfsf
        << left << setw(30) << "\tmaxImageExtent: " << right << printVkData(value.maxImageExtent) << endl // sfsf
        << left << setw(30) << "\tmaxImageArrayLayers: " << right << value.maxImageArrayLayers << endl    // sfsf
        << left << setw(30) << "\tsupportedTransforms: " << right << value.supportedTransforms << endl    // sfsf
        << left << setw(30) << "\tcurrentTransform: " << right << value.currentTransform << endl          // sfsf
        << left << setw(30) << "\tsupportedTransforms: " << right << value.supportedTransforms << endl    // sfsf
        << left << setw(30) << "\tsupportedUsageFlags: " << right << value.supportedUsageFlags << endl;   // sfsf
}

static void surfaceFormats(const std::vector<VkSurfaceFormatKHR> &vecFormat) {
    fprintf(stdout, "surface formats count: (%d)\n", vecFormat.size());
    int i = 0;
    for (const auto &v : vecFormat) {
        cout                                                                            // a
            << left << "\t" << right << i << endl                                       // sfsf
            << left << setw(30) << "\t\tformat: " << right << v.format << endl          // sfsf
            << left << setw(30) << "\t\tcolorSpace: " << right << v.colorSpace << endl; // sfsf
        ++i;
    }
}

static void surfacePresentModes(const std::vector<VkPresentModeKHR> &vecPresentMode) {
    fprintf(stdout, "surface present modes count: (%d)\n", vecPresentMode.size());
    int i = 0;
    for (const auto &v : vecPresentMode) {
        cout                                                                // a
            << left << setw(30) << "\tpresentMode: " << right << v << endl; // sfsf
    }
}
} // namespace printer

#endif
