cmake_minimum_required(VERSION 3.5)

set(PROJECT_VERSION 1.0.0)
set(LIB_API_VERSION 1)

project(LibQtAppBase VERSION ${PROJECT_VERSION})
set(LIB_NAME QtAppBase)

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

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Default build release
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Building ${LIB_NAME} by default in Release mode unless a different mode is specified")
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find required Qt5 or Qt6 dependencies
find_package(QT NAMES Qt6 Qt5)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network Core Concurrent Xml REQUIRED)

# Get all sources
file(GLOB_RECURSE Proj_SOURCES "src/*.c" "src/*.cpp")
file(GLOB_RECURSE Thirdparty_SOURCES "thirdparty/*.c" "thirdparty/*.cpp")

# Create library
add_library(${LIB_NAME} SHARED ${Proj_SOURCES} ${Thirdparty_SOURCES})
set_target_properties(${LIB_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${LIB_API_VERSION} PUBLIC_HEADER DIRECTORY "src/" FILES_MATCHING PATTERN "*.h")
# include headers
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

# include thirdparty libs
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/spdlog)
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty/spdlog/include)
include_directories(thirdparty)
include_directories(thirdparty/uriparser/include)

target_link_libraries(${LIB_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt${QT_VERSION_MAJOR}::Xml
        spdlog::spdlog)

# Install the library into a given folder
set(DEPLOY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/deploy)
install(TARGETS ${LIB_NAME} DESTINATION ${DEPLOY_DIR}/lib)

# Install headers for sources
install(DIRECTORY "src/" DESTINATION "${DEPLOY_DIR}/includes/${LIB_NAME}" FILES_MATCHING PATTERN "*.h")

# Install headers for thirdparty applications
install(DIRECTORY thirdparty/spdlog/include/spdlog DESTINATION ${DEPLOY_DIR}/includes/thirdparty)
install(FILES thirdparty/MultiSelectComboBox/MultiSelectComboBox.h DESTINATION ${DEPLOY_DIR}/includes/thirdparty)
install(FILES thirdparty/QJsonModel/QJsonModel.h DESTINATION ${DEPLOY_DIR}/includes/thirdparty)