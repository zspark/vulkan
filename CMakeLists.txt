cmake_minimum_required(VERSION 3.28)
project(vulkan-tutorial VERSION 0.0.1)

# Set preprocessor defines
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # doest not work;
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -D_USE_MATH_DEFINES")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler specific stuff
IF(MSVC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
ENDIF(MSVC)


message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "project name: ${PROJECT_NAME}")

find_package (Vulkan REQUIRED)
if(Vulkan_FOUND)
    message(STATUS "vulkan lib URL: ${Vulkan_LIBRARY}")
    #message(STATUS "vulkan path: ${Vulkan_LIBRARIES}")
    message(STATUS "vulkan header path: ${Vulkan_INCLUDE_DIRS}")
    include_directories(${Vulkan_INCLUDE_DIRS})
endif()
include_directories(external)
include_directories(external/glfw-3.3.9.bin.WIN64/include)
include_directories(external/glm)
include_directories(include)
link_directories(external/glfw-3.3.9.bin.WIN64/lib-vc2022)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
file(GLOB IMGUI_SRC "./external/imgui/*.cpp")

#message("IMGUI_SRC:${IMGUI_SRC}")

add_subdirectory(src/common)
add_subdirectory(src/presentation)
