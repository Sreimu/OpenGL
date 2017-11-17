cmake_minimum_required(VERSION 3.5)
project(openGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES main.cpp Shader.h Shader.cpp Camera.cpp Camera.h Mouse.cpp Mouse.h Light.cpp Light.h Material.cpp Material.h GLProgram.cpp GLProgram.h)
add_executable(openGL ${SOURCE_FILES})

include_directories(/usr/local/include/)
include_directories(./SOIL/src/)
LINK_DIRECTORIES(/usr/local/lib/)
#LINK_LIBRARIES(/Users/leisirui/ClionProjects/openGL/SOIL/lib/libSOIL.a)

#LINK_DIRECTORIES(/usr/local/Cellar/glew/)

FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
FIND_LIBRARY(IOKit_LIBRARY IOKit)
FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
MARK_AS_ADVANCED (COCOA_LIBRARY
        OpenGL_LIBRARY
        IOKit_LIBRARY
        CoreVideo_LIBRARY
        )
SET(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKit_LIBRARY} ${CoreVideo_LIBRARY})

target_link_libraries(openGL ${EXTRA_LIBS})
#find_package(OpenGL REQUIRED)
#target_link_libraries(openGL ${OPENGL_LIBRARIES})

#find_package(OpenGL REQUIRED)
#include_directories(${OPENGL_INCLUDE_DIR})
#link_libraries(${OPENGL_LIBRARIES})


set(TARGET_LIB
        "/usr/local/lib/libGLEW.dylib"
        "/usr/local/lib/libGLEW.1.13.dylib"
        "/usr/local/lib/libglfw3.a"
        "${openGL_SOURCE_DIR}/SOIL/lib/libSOIL.a"
        )

target_link_libraries(openGL ${TARGET_LIB})