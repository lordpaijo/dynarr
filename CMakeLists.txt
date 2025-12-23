cmake_minimum_required(VERSION 3.15)
project(dynarr
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Library
add_library(dynarr
    src/dynarr.c
)

# Create alias for consistent naming
add_library(dynarr::dynarr ALIAS dynarr)

target_include_directories(dynarr
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(dynarr
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

# Test executable (only build if this is the main project)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_executable(dumb_test
        tests/dumb.c
    )
    target_link_libraries(dumb_test
        PRIVATE dynarr
    )
endif()

# Installation
include(GNUInstallDirs)

# Install library
install(TARGETS dynarr
    EXPORT dynarrTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

# Install export targets
install(EXPORT dynarrTargets
    FILE dynarrTargets.cmake
    NAMESPACE dynarr::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dynarr
)

# Create config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/dynarrConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/dynarrConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dynarr
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/dynarrConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/dynarrConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/dynarrConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dynarr
)
