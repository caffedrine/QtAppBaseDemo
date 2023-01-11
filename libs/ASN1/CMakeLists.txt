cmake_minimum_required(VERSION 3.5)

set(PROJECT_VERSION 1.0.0)
set(LIB_API_VERSION 1)

project(ASN1 VERSION ${PROJECT_VERSION})
set(LIB_NAME ASN1Parser)

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
    message(STATUS "Building ${LIB_NAME} by default in Release mode unless a different mode is specified")
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find required Qt5 or Qt6 dependencies
find_package(QT NAMES Qt6 Qt5)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network Core Concurrent Xml REQUIRED)

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build/${CMAKE_BUILD_TYPE})

# include headers
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

# Get all sources
file(GLOB_RECURSE Proj_SOURCES "src/*.c" "src/*.cpp")

# Create library
add_library(${LIB_NAME} SHARED ${Proj_SOURCES})
set_target_properties(${LIB_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${LIB_API_VERSION})

target_link_libraries(${LIB_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt${QT_VERSION_MAJOR}::Xml)

# Install the library into a given folder
set(DEPLOY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/deploy)
install(TARGETS ${LIB_NAME} DESTINATION ${DEPLOY_DIR}/lib)

# Install headers for sources
install(FILES src/ASN1_Object.h DESTINATION ${DEPLOY_DIR}/includes/ASN1)
#install(DIRECTORY "src/" DESTINATION "${DEPLOY_DIR}/includes/${LIB_NAME}" FILES_MATCHING PATTERN "*.h")
