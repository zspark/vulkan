#include <stdlib.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
 
static void _errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}
 
static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    fprintf(stdout, "Key pressed, key:%d, scancode:%d, action:%d, mods:%d\n", key,scancode,action,mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && mods == GLFW_MOD_SHIFT)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static bool _initGLFW(GLFWwindow *&out, const char *caption, int w = 640, int h = 480) {
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
 
int main(int argc, char** argv) {
    GLFWwindow* window;
    if(!_initGLFW(window,"glfw window")){
        exit(EXIT_FAILURE);
    }
 
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    _deinitGLFW(window);
    exit(EXIT_SUCCESS);
}
