#ifndef _HEADER_GLFW_UTILS_
#define _HEADER_GLFW_UTILS_

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "vulkan-tutorial/printer.hpp"

namespace glfw {
static void _errorCallback(int error, const char *description) { fprintf(stderr, "[GLFW]Error: %s\n", description); }

static void _keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    fprintf(stdout, "Key pressed, key:%d, scancode:%d, action:%d, mods:%d\n", key, scancode, action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static GLFWwindow *createWindow(const char const *caption, int w = 640, int h = 480) {
    GLFWwindow *out = glfwCreateWindow(w, h, caption, NULL, NULL);
    if (!out) {
        fprintf(stderr, "failed to create glfw window!\n");
        return nullptr;
    }

    glfwSetKeyCallback(out, _keyCallback);
    glfwSetErrorCallback(_errorCallback);
    //
    // vulkan application does NOT need these.
    // glfwMakeContextCurrent(out);
    // glfwSwapInterval(1);

    return out;
}

static void init() {
    if (!glfwInit()) {
        fprintf(stderr, "[GLFW] init failed.\n");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    /**
     * When using Vulkan with GLFW, you need to create a GLFW window with the GLFW_NO_API hint, indicating that the window won't be used with OpenGL. Vulkan does not require an OpenGL context, and specifying GLFW_NO_API helps GLFW set up the window appropriately for Vulkan.
     *
     * Set GLFW to not create an OpenGL context, if not, there will be a error like: `Error: Vulkan: Window surface creation requires the window to have the client API set to GLFW_NO_API`;
     */
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

} // namespace glfw

#endif
