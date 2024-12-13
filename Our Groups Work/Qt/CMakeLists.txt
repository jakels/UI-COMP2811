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

# Conditionally set the Qt prefix path depending on the platform
if(WIN32)
    # Windows Qt installation path
    set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/mingw_64/lib/cmake")
elseif(APPLE)
    # macOS Qt installation path - adjust as necessary
    set(CMAKE_PREFIX_PATH "/Users/mondrgnedi/Qt/6.8.0/macos/lib/cmake")


endif()

# Find required Qt packages

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)

# Define all project source files, including new chart-related classes
set(PROJECT_SOURCES
        mainwindow.cpp
        mainwindow.h
        DatasetInterface.cpp
        DatasetInterface.h
        csv.hpp
        pollutantoverview.cpp
        fluorinatedpage.cpp
        Utils.cpp
        Utils.h
        compliancedashboard.cpp
        popspage.cpp
        popspage.h
        constants.h
        constants.h
        constants.cpp
        translation_manager.cpp
        translation_manager.h
)


# Define the executable for Qt 6 or lower
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(dash
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(dash SHARED ${PROJECT_SOURCES})
    else()
        add_executable(dash ${PROJECT_SOURCES})
    endif()
endif()


# Link Qt Widgets and Charts libraries
target_link_libraries(dash PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts)
configure_file(${CMAKE_SOURCE_DIR}/Water.csv ${CMAKE_BINARY_DIR}/Water.csv COPYONLY)

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

# Finalize the executable if using Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(dash)
endif()
