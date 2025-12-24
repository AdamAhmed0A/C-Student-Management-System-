cmake_minimum_required(VERSION 3.16)
project(SIS-Project)

# Set global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Copy .env file and CSV files to the build directory
if(EXISTS ${CMAKE_SOURCE_DIR}/.env)
    file(COPY ${CMAKE_SOURCE_DIR}/.env DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_SOURCE_DIR}/.env DESTINATION ${CMAKE_BINARY_DIR}/UniManage)
    file(COPY ${CMAKE_SOURCE_DIR}/.env DESTINATION ${CMAKE_BINARY_DIR}/UniManageCli)
endif()
file(COPY ${CMAKE_SOURCE_DIR}/UniManageCli/FileDB DESTINATION ${CMAKE_BINARY_DIR}/UniManageCli)

# Add subdirectories for core library, GUI, and CLI
add_subdirectory(UniManage)
add_subdirectory(UniManageCli)
