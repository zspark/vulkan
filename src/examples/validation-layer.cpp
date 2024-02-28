#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "vulkan-tutorial/glfw-utils.hpp"
#include "vulkan-tutorial/printer.hpp"
#include "vulkan-tutorial/vulkan-utils.hpp"

const char *const APP_NAME = "validation layer";
const char *const ENGINE_NAME = "ratel";

int main(int argc, char **argv) {
    GLFWwindow *_wnd{nullptr};
    VkInstance _ins{VK_NULL_HANDLE};

    glfw::init();

    if (!(_wnd = glfw::createWindow(APP_NAME))) {
        goto destroy;
    }

    if (!(_ins = vulkan::createInstance(APP_NAME, ENGINE_NAME))) {
        goto destroy;
    }

#ifndef NDEBUG
    VkDebugUtilsMessengerEXT _messenger{VK_NULL_HANDLE};
    if (!(_messenger = vulkan::createDebugUtilsMessenger(_ins))) {
        goto destroy;
    }
#endif

    while (!glfwWindowShouldClose(_wnd)) {
        glfwPollEvents();
    }


destroy:
    fprintf(stdout, "destroy called.");
#ifndef NDEBUG
    vulkan::destroyDebugUtilsMessenger(_ins, _messenger);
#endif
    if (_ins)
        vkDestroyInstance(_ins, nullptr);
    if (_wnd)
        glfwDestroyWindow(_wnd);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
