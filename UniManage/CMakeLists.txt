cmake_minimum_required(VERSION 3.16)
project(unimanagecore LANGUAGES CXX)

# Find necessary Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Sql)

# Automatically discover sources
file(GLOB_RECURSE CORE_SOURCES
    "controllers/*.cpp"
    "models/*.cpp"
    "database/*.cpp"
)
file(GLOB_RECURSE CORE_HEADERS
    "controllers/*.h"
    "models/*.h"
    "database/*.h"
)

# Add the core library
add_library(unimanagecore
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

# Link against Qt components
target_link_libraries(unimanagecore PRIVATE
    Qt6::Core
    Qt6::Sql
)

# Include the current source dir for headers
target_include_directories(unimanagecore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add the GUI application as a subdirectory
add_subdirectory(gui)
