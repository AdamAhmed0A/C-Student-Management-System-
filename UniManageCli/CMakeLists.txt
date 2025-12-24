cmake_minimum_required(VERSION 3.16)
project(UniManageCLI LANGUAGES CXX)

# Find necessary Qt components for a CLI application
find_package(Qt6 REQUIRED COMPONENTS Core)


# Automatically discover sources in the current folder
file(GLOB CLI_SOURCES "*.cpp")
file(GLOB CLI_HEADERS "*.h")
file(GLOB_RECURSE CLI_PERSISTENCE_SOURCES "persistence/*.cpp")

# Add the executable for the CLI
add_executable(unimanage_cli
    ${CLI_SOURCES}
    ${CLI_HEADERS}
    ${CLI_PERSISTENCE_SOURCES}
)

# Link against the core library
link_directories(${CMAKE_BINARY_DIR}/../UniManage)
target_link_libraries(unimanage_cli PRIVATE
    Qt6::Core
    unimanagecore
)

# Add the directory containing the core library for the CLI controller to find the data files
target_include_directories(unimanage_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../UniManage)

# Copy the FileDB folder to the build directory alongside the executable
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/FileDB" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/FileDB")
