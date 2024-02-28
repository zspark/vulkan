#include <iostream>
#include <optional>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define GLFW_INCLUDE_VULKAN /// to encircle APIs such as `glfwCreateWindowSurface`;
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "vulkan-tutorial/glfw-utils.hpp"
#include "vulkan-tutorial/printer.hpp"
#include "vulkan-tutorial/vulkan-utils.hpp"

const char *const APP_NAME = "physical device";
const char *const ENGINE_NAME = "ratel";

int main(int argc, char **argv) {

    GLFWwindow *_wnd{nullptr};
    VkInstance _ins{nullptr};
    VkSurfaceKHR _surface{nullptr};

    glfw::init();

    if (!(_wnd = glfw::createWindow(APP_NAME))) {
        goto destroy;
    }

    if (!(_ins = vulkan::createInstance(APP_NAME, ENGINE_NAME))) {
        goto destroy;
    }

    if (!(_surface = vulkan::createSurface(_ins, _wnd))) {
        goto destroy;
    }

    while (!glfwWindowShouldClose(_wnd)) {
        /**
         * Error: Cannot swap buffers of a window that has no OpenGL or OpenGL ES context
         * so, we can't use the api below any more.
         *
         * glfwSwapBuffers(_wnd);
         */
        glfwPollEvents();
    }

destroy:
    fprintf(stdout, "destroy called.");
    if (_surface)
        vkDestroySurfaceKHR(_ins, _surface, nullptr);
    if (_ins)
        vkDestroyInstance(_ins, nullptr);
    if (_wnd)
        glfwDestroyWindow(_wnd);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}