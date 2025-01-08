cmake_minimum_required(VERSION 3.12)
project(Qtris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

qt_add_executable(Qtris 
    main.cpp
    ui/main_window.cpp
    ui/menu_bar.cpp

    ui/play/main_view.cpp
    ui/play/play_area.cpp
    ui/play/render_area.cpp
    ui/play/next_piece_view.cpp
    ui/play/statistics_view.cpp
    ui/play/piece_manager.cpp

    ui/menus/select_level.cpp
    
    game_manager/play_field.cpp
    game_manager/main_loop.cpp
    game_manager/current_piece.cpp
    game_manager/core_runtime.cpp

    data_manager/piece_data.cpp
    data_manager/settings.cpp
    data_manager/runtime_data.cpp

    utils/event.h)

qt_add_resources(
    Qtris "images" 
    PREFIX "/" 
    FILES ../assets/bg.png)

qt_add_resources(
    Qtris "fonts" 
    PREFIX "/" 
    FILES ../assets/press_start_2P.ttf)

target_link_libraries(Qtris PRIVATE Qt6::Widgets)

set_target_properties(Qtris PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)