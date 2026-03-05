set(CMAKE_CXX_STANDARD 20)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)  
set(CMAKE_CXX_EXTENSIONS OFF)



add_library(Debug)

target_include_directories(Debug
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(Debug
    PUBLIC
    FILE_SET debug_module TYPE CXX_MODULES FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Debug.cppm
)
