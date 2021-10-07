cmake_minimum_required(VERSION 3.14)

project(
    "Recursive Variant Authority"
    VERSION 1.0.0
    DESCRIPTION "A Lightweight Tuple Implementation for Modern C++"
    HOMEPAGE_URL "https://github.com/codeinred/rva/"
    LANGUAGES CXX
)

add_library(rva INTERFACE)
add_library(rva::rva ALIAS rva)

# For ${CMAKE_INSTALL_<DIR>} variables that are standarized
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_compile_features(
    rva
    INTERFACE
    cxx_std_20
)

target_include_directories(
    rva
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Installation
# See: https://dominikberner.ch/cmake-interface-lib/

install(
    TARGETS rva
    EXPORT rva_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "rvaConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/rvaConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/rvaConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/rva/cmake
)

install(
    EXPORT rva_Targets
    FILE rvaTargets.cmake
    NAMESPACE rva::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/rva/cmake
)

install(FILES "${PROJECT_BINARY_DIR}/rvaConfig.cmake"
              "${PROJECT_BINARY_DIR}/rvaConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/rva/cmake
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/rva
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


include(helper)
if(PROJECT_IS_TOP_LEVEL)
    include(CTest)

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    # Add all files in directory test as tests, and link against rva and
    # threads

    #if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    #    target_compile_options(
    #        test_kilo_tuple
    #        PRIVATE
    #        -fbracket-depth=1000)
    #endif()

    set(BENCHMARK_ENABLE_TESTING OFF)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF)

    find_or_fetch(
        fmt
        https://github.com/fmtlib/fmt.git
        master)
    always_fetch(
        Catch2
        https://github.com/catchorg/catch2.git
        devel)
    FetchContent_MakeAvailable(${remote_dependencies})

    add_executable(
        test_rva
        test/test_rva.cpp)
    target_link_libraries(
        test_rva
        rva::rva
        fmt::fmt
        Catch2::Catch2WithMain)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(CTest)
    include(Catch)
    catch_discover_tests(test_rva)
endif()


