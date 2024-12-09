cmake_minimum_required(VERSION 3.16)

# Project name and version
project(dash VERSION 0.1 LANGUAGES CXX)

# Enable automatic handling of UI, MOC, and RCC files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

# List all source and header files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    datavisualizationpage.cpp
    datavisualizationpage.h
    pollutantoverview.cpp
    pollutantoverview.h
    fluorinatedpage.cpp
    fluorinatedpage.h
    mainwindow.ui
)

# Create the executable
qt_add_executable(dash
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    fluorinatedpage.h fluorinatedpage.cpp
)

# Link the Qt Widgets and Charts libraries
target_link_libraries(dash PRIVATE Qt6::Widgets Qt6::Charts)

# Set properties for macOS/Windows bundles (optional)
set_target_properties(dash PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Installation rules
include(GNUInstallDirs)
install(TARGETS dash
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
set_source_files_properties(pollutantoverview.cpp PROPERTIES SKIP_AUTOMOC ON)

# Finalize the executable for Qt6
qt_finalize_executable(dash)
