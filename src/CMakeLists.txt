set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/)
#set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs/)
#message(STATUS "LIBS_DIR: ${LIBS_DIR}")

function(buildExample EXAMPLE_PATH EXAMPLE_NAME)
    file(GLOB SOURCE ${EXAMPLE_PATH}/${EXAMPLE_NAME}.cpp)

    add_executable(${EXAMPLE_NAME} ${SOURCE} ${IMGUI_SRC})
    target_link_libraries(${EXAMPLE_NAME} glfw3 ${Vulkan_LIBRARY} ${WINLIBS})

    # Specify the output directory for the executable
    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    )

    #file(MAKE_DIRECTORY ${OUTPUT_DIR})

    # Install the executable to the 'bin' directory
    #install(TARGETS ${EXAMPLE_NAME} DESTINATION bin)
    #file(COPY ${EXAMPLE_NAME} DESTINATION ${OUTPUT_DIR})

endfunction()

# Build all examples
function(buildExamples folder files)
endfunction()

set(presentations
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
foreach(file ${presentations})
    buildExample("presentation" ${file})
endforeach()

set(pipelines
    render-pass
    shader-module
    pipeline-layout
    pipeline
    framebuffer
    command-pool-buffer
    fence-semaphore
    record-command
    render
)
foreach(file ${pipelines})
    buildExample("pipeline" ${file})
endforeach()
