#set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/)
#set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs/)
#message(STATUS "LIBS_DIR: ${LIBS_DIR}")

function(buildExample EXAMPLE_NAME)
    file(GLOB SOURCE ${EXAMPLE_NAME}.cpp)

    add_executable(${EXAMPLE_NAME} ${SOURCE} ${IMGUI_SRC})
    target_link_libraries(${EXAMPLE_NAME} glfw3 ${Vulkan_LIBRARY} ${WINLIBS})

    #file(MAKE_DIRECTORY ${OUTPUT_DIR})

endfunction(buildExample)

# Build all examples
function(buildExamples)
	foreach(EXAMPLE ${EXAMPLES})
		buildExample(${EXAMPLE})
	endforeach(EXAMPLE)
endfunction(buildExamples)

set(EXAMPLES
    empty
    glfw-usage
    layers-and-extensions
    instance
    physical-device
    queue-families
    logical-device
    surface
    swap-chain
    image-view
    validation-layer
)

buildExamples()
