cmake_minimum_required(VERSION 3.16)

# Define the project with both C and C++ languages
project(dash VERSION 0.1 LANGUAGES C CXX)

# Enable Qt's automatic features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the path to Qt's CMake configuration
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/mingw_64/lib/cmake")

# Find the required Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Define all project source files, including sqlite3.c
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        DatasetInterface.cpp
        DatasetInterface.h
        csv.hpp
        sqlite3.c  # Added SQLite3 source file
)

# Define the executable based on Qt version
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(dash
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
    # You can add additional Qt-specific properties here if needed
else()
    if(ANDROID)
        add_library(dash SHARED
                ${PROJECT_SOURCES}
        )
        # Define properties for Android with Qt 5 if necessary
    else()
        add_executable(dash
                ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link Qt Widgets library
target_link_libraries(dash PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Set bundle properties for macOS and other platforms
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.dash)
endif()

set_target_properties(dash PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

# Specify installation directories
include(GNUInstallDirs)
install(TARGETS dash
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize the executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(dash)
endif()
