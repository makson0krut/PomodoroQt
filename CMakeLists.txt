cmake_minimum_required(VERSION 3.16)
project(PomodoroQt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(MACOSX_BUNDLE_BUNDLE_NAME "Pomodoro")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.maksim.pomodoro")
set(MACOSX_BUNDLE_ICON_FILE "AppIcon.icns")

add_executable(PomodoroQt MACOSX_BUNDLE
        main.cpp
        pomodorowindow.cpp
        mainmenuwindow.cpp
        statswindow.cpp
        taskwindow.cpp
        circulartimerwidget.cpp
        statschartwidget.cpp
        statschartwidget.h
)
set_source_files_properties(AppIcon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
target_sources(PomodoroQt PRIVATE AppIcon.icns)


set_target_properties(PomodoroQt PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
)

target_link_libraries(PomodoroQt PRIVATE Qt6::Widgets)