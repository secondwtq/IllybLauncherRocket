cmake_minimum_required(VERSION 3.1)
project(IllybLauncher)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

set(SOURCE_FACER_EVENT FacerEvent.hxx FacerEventKey.hxx FacerEvent.cxx
    FacerEventPortGLFW.hxx FacerEventPortGLFW.cxx FacerEventPortRocket.hxx FacerEventPortRocket.cxx)

set(SOURCE_FILES ${SOURCE_FACER_EVENT} main.cxx ShellRenderInterfaceOpenGL.cpp config.hxx config.cxx
    CABALPortRocket.hxx CABALPortRocket.cxx INIBinding.hxx INIBinding.cxx CABAL.hxx CABAL.cxx
    ShellSystemInterface.cpp ShellSystemInterface.h thirdpt/stb.cxx thirdpt/luajit.hxx thirdpt/glfw.hxx thirdpt/simpleini.hxx
        thirdpt/luabridge.hxx thirdpt/stb_image.hxx platform.h thirdpt/codecvt.hxx GLFWFunctions.cxx GLFWFunctions.hxx main.hxx
        RocketEventListener.hxx RocketEventListener.cxx)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(LuaJIT)
if(LUAJIT_FOUND)
    include_directories(${LUAJIT_INCLUDE_DIR})
    list(APPEND HOWARD_LIBS ${LUAJIT_LIBRARIES})
endif()

find_package(PhysFS)
if(PHYSFS_FOUND)
    include_directories(${PHYSFS_INCLUDE_DIR})
    list(APPEND HOWARD_LIBS ${PHYSFS_LIBRARY})
endif()

find_package(GLFW)
if(GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIR})
    list(APPEND HOWARD_LIBS ${GLFW_LIBRARIES})
endif()

set(LIBROCKET_STATIC TRUE CACHE BOOL "Are you linking against libRocket statically?")
if(LIBROCKET_STATIC)
    add_definitions(-DROCKET_STATIC_LIB)

    find_package(Freetype)
    if(FREETYPE_FOUND)
        include_directories(${FREETYPE_INCLUDE_DIRS})
        list(APPEND HOWARD_LIBS ${FREETYPE_LIBRARIES})
    endif()
endif(LIBROCKET_STATIC)

find_package(LibRocket)
if(LIBROCKET_FOUND)
    include_directories(${LIBROCKET_INCLUDE_DIRS})
    list(APPEND HOWARD_LIBS ${LIBROCKET_LIBRARIES})
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

    list(APPEND HOWARD_LIBS ${EXTRA_LIBS_APPLE})

    # LuaJIT needs this
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")

endif(APPLE)

if(WIN32)
    set(EXTRA_LIBS_WIN32 opengl32)
    list(APPEND HOWARD_LIBS ${EXTRA_LIBS_WIN32})

    if(MINGW)
        # for RC files
        # www.cmake.org/Bug/view.php?id=4068
        # already fixed :)

        set(MINGW_STATIC FALSE CACHE BOOL "If you are using MinGW32, should the runtime linked staticlly?")
        if (MINGW_STATIC)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic -static")
        endif(MINGW_STATIC)
    endif(MINGW)

    list(APPEND SOURCE_FILES ver.rc)

    set(DISABLE_WIN32_CONSOLE TRUE CACHE BOOL "Eliminate the shell window in Windows.")
    # stackoverflow.com/questions/4441551/how-to-stop-mingw-g-opening-a-console-window-in-windows
    if (MINGW AND DISABLE_WIN32_CONSOLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
        # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    endif(MINGW AND DISABLE_WIN32_CONSOLE)
endif(WIN32)

add_executable(IllybLauncher ${SOURCE_FILES})

include_directories(.)
include_directories(third_party/LuaBridge/Source)

target_link_libraries(IllybLauncher ${HOWARD_LIBS})
