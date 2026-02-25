cmake_minimum_required(VERSION 3.20)
project(learnCpp LANGUAGES CXX)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(SRC_DIR_LIST 
        ${CMAKE_SOURCE_DIR}/Cpp_11_14_Houjie
        ${CMAKE_SOURCE_DIR}/MemoryManagement_Houjie
    )

foreach(SRC_DIR ${SRC_DIR_LIST})
    file(GLOB SRC_SUBDIR_LIST RELATIVE ${SRC_DIR} ${SRC_DIR}/*) # 获取文件夹下的文件夹的名字
    foreach(SRC_SUBDIR ${SRC_SUBDIR_LIST})
        add_subdirectory(${SRC_DIR}/${SRC_SUBDIR})
    endforeach()
endforeach()