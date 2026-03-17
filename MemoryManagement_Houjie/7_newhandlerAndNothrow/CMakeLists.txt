cmake_minimum_required(VERSION 3.20)
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
message(STATUS "当前文件夹名: ${CURRENT_FOLDER_NAME}")
project(${CURRENT_FOLDER_NAME})
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

