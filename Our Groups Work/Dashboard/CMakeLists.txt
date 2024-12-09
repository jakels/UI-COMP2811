cmake_minimum_required(VERSION 3.16)
project(dash VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conditionally set the Qt prefix path depending on the platform
if(WIN32)
    # Windows Qt installation path
    set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/mingw_64/lib/cmake")
elseif(APPLE)
    # macOS Qt installation path
    # Adjust this path to match your local Qt installation.
    set(CMAKE_PREFIX_PATH "/Users/<username>/Qt/6.8.1/clang_64/lib/cmake")
endif()

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        DatasetInterface.cpp
        DatasetInterface.h
        csv.hpp
        sqlite3.c
)

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

target_link_libraries(dash PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

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

include(GNUInstallDirs)

install(TARGETS dash
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(dash)
endif()
