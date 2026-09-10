cmake_minimum_required(VERSION 3.16)
project(Vortex3D_Core VERSION 0.0.4 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

add_library(vortex_core_headers INTERFACE)
target_include_directories(vortex_core_headers INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Engine>
)
target_compile_features(vortex_core_headers INTERFACE cxx_std_20)

add_executable(vortex_core Tests/main.cpp)
target_link_libraries(vortex_core PRIVATE vortex_core_headers)

if(MSVC)
    target_compile_options(vortex_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(vortex_core PRIVATE
        -Wall -Wextra -Wpedantic -Wconversion -Wshadow
    )
endif()

enable_testing()
add_test(NAME vortex_foundation_tests COMMAND vortex_core)
