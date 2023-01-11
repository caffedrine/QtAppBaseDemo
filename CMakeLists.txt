cmake_minimum_required(VERSION 3.16)

project(qtappbase_app LANGUAGES CXX C)

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
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network Core Concurrent REQUIRED)

# Add ASN1 Parser library
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/ASN1)

# Find the precompiled libQtAppBase library and installit when --target QtAppBase
#set(LIB_QTAPPBASE_NAME QtAppBAse)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/${LIB_QTAPPBASE_NAME})
##if(TARGET ${PROJECT_NAME})
##    set(LIB_QTAPPBASE_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/${LIB_QTAPPBASE_NAME}/build/deploy")
##    # find_library(QTAPPBASE_LIB NAMES ${LIB_QTAPPBASE_NAME} lib${LIB_QTAPPBASE_NAME} HINTS "${CMAKE_BINARY_DIR}/libs/QtAppBase" "${CMAKE_BINARY_DIR}/libs/QtAppBase" REQUIRED)
##    # include_directories(${LIB_QTAPPBASE_DIR}/includes/thirdparty)
##    # include_directories(${LIB_QTAPPBASE_DIR}/includes)
##endif()

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Concurrent
        ASN1Parser)

## Deploy base library to executable folder
#file(GLOB QTAPPBASE_LIBS "${LIB_QTAPPBASE_DIR}/lib/*")
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${QTAPPBASE_LIBS} "${CMAKE_BINARY_DIR}/" COMMENT "Copying libQtAppBase to output directory")
