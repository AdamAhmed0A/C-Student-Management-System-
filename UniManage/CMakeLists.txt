cmake_minimum_required(VERSION 3.20)
project(UniManage LANGUAGES CXX)

# Use C++17 (adjust if needed)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt automoc/autouic/autorcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find Qt6 modules you need
find_package(Qt6 COMPONENTS Core Widgets Sql REQUIRED)

# Only scan YOUR project
file(GLOB_RECURSE UM_SOURCES
  "${CMAKE_SOURCE_DIR}/gui/*.cpp"
  "${CMAKE_SOURCE_DIR}/controllers/*.cpp"
  "${CMAKE_SOURCE_DIR}/models/*.cpp"
  "${CMAKE_SOURCE_DIR}/database/*.cpp"
)

file(GLOB_RECURSE UM_HEADERS
  "${CMAKE_SOURCE_DIR}/gui/*.h"
  "${CMAKE_SOURCE_DIR}/controllers/*.h"
  "${CMAKE_SOURCE_DIR}/models/*.h"
  "${CMAKE_SOURCE_DIR}/database/*.h"
)

add_executable(UniManage
  ${UM_SOURCES}
  ${UM_HEADERS}
)

# Add include dirs to match your source include style (adjust paths if needed)
target_include_directories(UniManage PRIVATE
  "${CMAKE_SOURCE_DIR}/"
  "${CMAKE_SOURCE_DIR}/gui"
  "${CMAKE_SOURCE_DIR}/controllers"
  "${CMAKE_SOURCE_DIR}/models"
  "${CMAKE_SOURCE_DIR}/database"
)

# Link Qt libraries
target_link_libraries(UniManage PRIVATE
  Qt6::Core
  Qt6::Widgets
  Qt6::Sql
)

