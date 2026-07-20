cmake_minimum_required(VERSION 3.16)
project(QtHelloWorld)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(PomodoroQt main.cpp
        pomodorowindow.cpp
        pomodorowindow.h
        mainmenuwindow.cpp
        mainmenuwindow.h
        circulartimerwidget.cpp
        circulartimerwidget.h
        statswindow.cpp
        statswindow.h
        taskwindow.cpp
        taskwindow.h)

target_link_libraries(PomodoroQt PRIVATE Qt6::Widgets)