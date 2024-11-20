cmake_minimum_required(VERSION 3.12)
project(Qtris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(Qtris 
    main.cpp
    ui/main_view.cpp
    ui/menu_bar.cpp)

qt_add_resources(
    Qtris "images" 
    PREFIX "/images" 
    FILES ../assets/bg.png)

target_link_libraries(Qtris PRIVATE Qt6::Widgets)

set_target_properties(Qtris PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)