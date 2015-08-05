cmake_minimum_required(VERSION 3.2)
project(IllybLauncher)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

set(SOURCE_FACER_EVENT FacerEvent.hxx FacerEventKey.hxx FacerEvent.cxx
    FacerEventPortGLFW.hxx FacerEventPortGLFW.cxx FacerEventPortRocket.hxx FacerEventPortRocket.cxx)

set(SOURCE_FILES ${SOURCE_FACER_EVENT} main.cxx ShellRenderInterfaceOpenGL.cpp config.hxx config.cxx
    ShellSystemInterface.cpp ShellFileInterface.cpp INIBinding.hxx INIBinding.cxx)
add_executable(IllybLauncher ${SOURCE_FILES} RocketEventInstancer.hxx RocketEventListener.hxx RocketEventListener.cxx)

set(HOWARD_LIBS ${HOWARD_LIBS} glfw3)

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

set(ROCKET_CORELIBS ${ROCKET_CORELIBS} RocketCore RocketControls RocketDebugger)
set(ROCKET_LUALIBS ${ROCKET_LUALIBS} RocketCoreLua RocketControlsLua)
set(LUA_LIBRARIES ${LUA_LIBRARIES} luajit-5.1)

set(HOWARD_LIBS ${HOWARD_LIBS} ${ROCKET_CORELIBS} ${ROCKET_LUALIBS} ${LUA_LIBRARIES})
target_link_libraries(IllybLauncher ${HOWARD_LIBS})
