cmake_minimum_required(VERSION 3.16)
project(ElevatorLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)   # processes .ui files automatically

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp

    # Core
    elevatorenums.h
    elevatorrequest.h
    ischedulingstrategy.h
    directionalschedulingstrategy.h   directionalschedulingstrategy.cpp
    elevatorscheduler.h               elevatorscheduler.cpp
    elevatorcabin.h                   elevatorcabin.cpp
    elevatordoors.h                   elevatordoors.cpp
    elevatorcontroller.h              elevatorcontroller.cpp
    elevatorsystem.h                  elevatorsystem.cpp

    # Utils
    enumstrings.h                     enumstrings.cpp

    # GUI — forms
    mainwindow.ui
    cabinpanel.ui
    statuspanel.ui
    logpanel.ui

    # GUI — code
    logpanel.h                        logpanel.cpp
    statuspanel.h                     statuspanel.cpp
    cabinpanel.h                      cabinpanel.cpp
    floorwidget.h                     floorwidget.cpp
    elevatorview.h                    elevatorview.cpp
    mainwindow.h                      mainwindow.cpp

    # Resources
    resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ElevatorLab
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(ElevatorLab WIN32
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(ElevatorLab PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ElevatorLab)
endif()
