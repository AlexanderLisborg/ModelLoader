
cmake_minimum_required(VERSION 4.2.0)
project(
    ModelLoader
    VERSION 1.0
    DESCRIPTION "A simple model loader for displaying hand ins for a computer graphics course at Uppsala Universitet. Authored by Alexander Lisborg."
    LANGUAGES CXX C
)

set_property(TARGET  PROPERTY C_STANDARD 20)
set(CMAKE_CXX_STANDARD_20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Organize system wide output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)  
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Compiling with GCC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Compiling with Clang")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Compiling with MSVC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
    message(STATUS "Compiling with Intel")
endif ()

# Subdirectories (MODULES)
#add_subdirectory(Modules/Debug)

add_executable(ModelLoader)

target_compile_features(ModelLoader PUBLIC cxx_std_20)
target_compile_definitions(ModelLoader PUBLIC SRCPATH="${CMAKE_CURRENT_SOURCE_DIR}")
# GLFW
add_subdirectory(Modules/glfw-3.4)

# OpenGL
find_package(OpenGL REQUIRED)

target_link_libraries(ModelLoader
    PRIVATE
    OpenGL::GL
    glfw
)

target_include_directories(ModelLoader
    PRIVATE
    Modules/glad/include
    Modules/Debug/include
)

target_sources(ModelLoader
    PUBLIC
    FILE_SET all_my_modules TYPE CXX_MODULES FILES
        Modules/Debug/Debug.cppm
        Modules/Common/Common.cppm
    PRIVATE
        # glad
        Modules/glad/src/glad.c
        # main
        Application/src/Main.cpp
)


#target_compile_definitions(ModelLoader PRIVATE SRCPATH="${CMAKE_CURRENT_SOURCE_DIR}")
