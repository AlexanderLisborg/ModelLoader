set(CMAKE_CXX_STANDARD 20)  
set(CMAKE_CXX_STANDARD_REQUIRED ON)  
set(CMAKE_CXX_EXTENSIONS OFF)



add_library(Common)

target_sources(Common
    PUBLIC
    FILE_SET common_module TYPE CXX_MODULES FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Obvc.cppm
    ${CMAKE_CURRENT_SOURCE_DIR}/Common.cppm
)
