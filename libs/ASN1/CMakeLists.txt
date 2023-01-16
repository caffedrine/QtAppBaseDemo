cmake_minimum_required(VERSION 3.9)

project(ASN1Parser LANGUAGES CXX C VERSION 1.0.1 DESCRIPTION "ASN1 Encoding Parser Library")

include(GNUInstallDirs)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Get all sources
file(GLOB_RECURSE LIB_SOURCES "src/*.c" "src/*.cpp")

add_library(ASN1Parser SHARED ${LIB_SOURCES})
add_library(ASN1::ASN1Parser ALIAS ASN1Parser)
set_target_properties(ASN1Parser PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)
include_directories(src)

configure_file(ASN1Parser.pc.in ASN1Parser.pc @ONLY)

# Public API headers
target_include_directories(ASN1Parser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:src>)

install(TARGETS ASN1Parser LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS ASN1Parser EXPORT ASN1ParserConfig LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
export(TARGETS ASN1Parser FILE "${CMAKE_CURRENT_BINARY_DIR}/ASN1ParserConfig.cmake")
install(EXPORT ASN1ParserConfig DESTINATION "${CMAKE_INSTALL_LIBDIR}/ASN1Parser/cmake" NAMESPACE ASN1::)
install(DIRECTORY src DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${CMAKE_BINARY_DIR}/ASN1Parser.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)