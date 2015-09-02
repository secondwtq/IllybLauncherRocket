cmake_minimum_required(VERSION 3.1)
project(IllybLauncher)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

set(SOURCE_FACER_EVENT FacerEvent.hxx FacerEventKey.hxx FacerEvent.cxx
    FacerEventPortGLFW.hxx FacerEventPortGLFW.cxx FacerEventPortRocket.hxx FacerEventPortRocket.cxx)

set(SOURCE_FILES ${SOURCE_FACER_EVENT} main.cxx ShellRenderInterfaceOpenGL.cpp config.hxx config.cxx
    CABALPortRocket.hxx CABALPortRocket.cxx INIBinding.hxx INIBinding.cxx CABAL.hxx CABAL.cxx
    ShellSystemInterface.cpp ShellSystemInterface.h stb.cxx)
add_executable(IllybLauncher ${SOURCE_FILES} RocketEventInstancer.hxx RocketEventListener.hxx RocketEventListener.cxx)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(LuaJIT)
if(LUAJIT_FOUND)
    include_directories(${LUAJIT_INCLUDE_DIR})
    set(HOWARD_LIBS ${HOWARD_LIBS} ${LUAJIT_LIBRARIES})
endif()

find_package(PhysFS)
if(PHYSFS_FOUND)
    include_directories(${PHYSFS_INCLUDE_DIR})
    set(HOWARD_LIBS ${HOWARD_LIBS} ${PHYSFS_LIBRARY})
endif()

find_package(GLFW)
if(GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIR})
    set(HOWARD_LIBS ${HOWARD_LIBS} ${GLFW_LIBRARIES})
endif()

find_package(LibRocket)
if(LIBROCKET_FOUND)
    include_directories(${LIBROCKET_INCLUDE_DIRS})
    set(HOWARD_LIBS ${HOWARD_LIBS} ${LIBROCKET_LIBRARIES})
endif()

if(APPLE)

    find_library(FRAMEWORK_COCOA Cocoa)
    find_library(FRAMEWORK_GL OpenGL)
    find_library(FRAMEWORK_IOKIT IOKit)
    find_library(FRAMEWORK_CORE_VIDEO CoreVideo)
    find_library(FRAMEWORK_FREETYPE freetype)
    # mark_as_advanced(FRAMEWORK_COCOA)
    set(EXTRA_LIBS_APPLE ${FRAMEWORK_COCOA} ${FRAMEWORK_GL} ${FRAMEWORK_IOKIT}
        ${FRAMEWORK_CORE_VIDEO} ${FRAMEWORK_FREETYPE})

    set(HOWARD_LIBS ${HOWARD_LIBS} ${EXTRA_LIBS_APPLE})

    # LuaJIT needs this
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")

endif(APPLE)

target_link_libraries(IllybLauncher ${HOWARD_LIBS})
