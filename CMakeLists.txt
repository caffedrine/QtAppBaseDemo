cmake_minimum_required(VERSION 3.16)

project(QtAppBaseDemo DESCRIPTION "Qt demo application" VERSION 1.0.0 LANGUAGES CXX C)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_output)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_output)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Enable debugging options
set(VERBOSE ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Assume error console to print out data always
set(QT_ASSUME_STDERR_HAS_CONSOLE 1)

# Default build release
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Building ${PROJECT_NAME} by default in Release mode unless a different mode is specified")
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find required Qt5 or Qt6 dependencies
find_package(QT NAMES Qt6 Qt5)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network Core Concurrent Xml REQUIRED)

# Create config file based on CMake definitions
configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/Config.h.in ${CMAKE_CURRENT_LIST_DIR}/src/gendata/Config.h)

# Add ASN1 Parser library
# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/ASN1)

# Add core library
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/QtAppBase)

## Just link a precompiled core library
#set(LIB_QTAPPBASE_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/${LIB_QTAPPBASE_NAME}/build/deploy")
#find_library(QTAPPBASE_LIB NAMES ${LIB_QTAPPBASE_NAME} lib${LIB_QTAPPBASE_NAME} HINTS "${CMAKE_BINARY_DIR}/libs/${LIB_QTAPPBASE_NAME}" REQUIRED)
#include_directories(${LIB_QTAPPBASE_DIR}/includes/thirdparty)
#include_directories(${LIB_QTAPPBASE_DIR}/includes)

# All source code files
set(ALL_SOURCES src/main.cpp src/MainApp.cpp src/ApplSettings.cpp ui/MainConsole.cpp ui/MainWindow.cpp ui/MainWindow.ui)

# Proj executable files - if this is not added, application will launch a console simultaneously with the gui app.
IF (WIN32)
    # Icon How-To: https://forum.qt.io/topic/122430/setting-up-the-application-information-in-cmake/4
    # set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/res/win.rc")
    # add_executable(${PROJECT_NAME} WIN32 ${ALL_SOURCES} ${APP_ICON_RESOURCE_WINDOWS})
    add_executable(${PROJECT_NAME} WIN32 ${ALL_SOURCES})
ELSE()
    add_executable(${PROJECT_NAME} ${ALL_SOURCES})
ENDIF()

target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Xml
        Qt${QT_VERSION_MAJOR}::Concurrent
        base::QtAppBase
        ASN1::ASN1Parser)

include(deploy.cmake)