cmake_minimum_required(VERSION 3.2)
project(IllybLauncher)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp ShellRenderInterfaceOpenGL.cpp ShellSystemInterface.cpp ShellFileInterface.cpp)
add_executable(IllybLauncher ${SOURCE_FILES})

set(HOWARD_LIBS ${HOWARD_LIBS} glfw3)

if(APPLE)

    find_library(FRAMEWORK_COCOA Cocoa)
    find_library(FRAMEWORK_GL OpenGL)
    find_library(FRAMEWORK_IOKIT IOKit)
    find_library(FRAMEWORK_CORE_VIDEO CoreVideo)
    # mark_as_advanced(FRAMEWORK_COCOA)
    set(EXTRA_LIBS_APPLE ${FRAMEWORK_COCOA} ${FRAMEWORK_GL} ${FRAMEWORK_IOKIT}
        ${FRAMEWORK_CORE_VIDEO})

    set(HOWARD_LIBS ${HOWARD_LIBS} ${EXTRA_LIBS_APPLE})

endif(APPLE)

set(HOWARD_LIBS ${HOWARD_LIBS} RocketCore RocketControls RocketDebugger)
target_link_libraries(IllybLauncher ${HOWARD_LIBS})
