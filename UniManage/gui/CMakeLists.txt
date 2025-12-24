cmake_minimum_required(VERSION 3.16)
project(UniManageGUI LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Sql)

# Automatically discover sources in the current folder
file(GLOB_RECURSE GUI_SOURCES "*.cpp")
file(GLOB_RECURSE GUI_HEADERS "*.h")
file(GLOB_RECURSE GUI_UI "*.ui")

add_executable(unimanage_gui
    ${GUI_SOURCES}
    ${GUI_HEADERS}
    ${GUI_UI}
)

# Link against the core library and Qt components
target_link_libraries(unimanage_gui PRIVATE
    unimanagecore
    Qt6::Widgets
    Qt6::Gui
    Qt6::Sql
)

# Set output directory to UniManage folder
set_target_properties(unimanage_gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/UniManage
)
