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

static bool _initGLFW(GLFWwindow *&out, const char const *caption, int w = 640, int h = 480) {
    if (!glfwInit()){
        return false;
    }
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
    out = glfwCreateWindow(w, h, caption, NULL, NULL);
    if (!out) {
        glfwTerminate();
        return false;
    }
 
    glfwSetKeyCallback(out, _keyCallback);
    glfwSetErrorCallback(_errorCallback);
    glfwMakeContextCurrent(out);
    glfwSwapInterval(1);
    return true;
}

static bool _deinitGLFW(GLFWwindow* window){
    glfwDestroyWindow(window);
    glfwTerminate();
    return true;
}

static void _printLayers() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> _availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, _availableLayers.data());

    printer::layers( _availableLayers);
}

static void _printExtensions(){
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> _extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, _extensions.data());

    printer::extensions( _extensions);
}

int main(int argc, char** argv) {
    GLFWwindow* window;
    if(!_initGLFW(window,APP_NAME)){
        exit(EXIT_FAILURE);
    }

    _printLayers();
    _printExtensions();
 
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    _deinitGLFW(window);
    exit(EXIT_SUCCESS);
}

