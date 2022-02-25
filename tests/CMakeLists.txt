# CMake version string
cmake_minimum_required(VERSION 3.0)

# Find Qt libraries
find_package(Qt5 ${QT_REQUIRED_VERSION} COMPONENTS Test REQUIRED)

# Project
project(test_kjarni)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include
HEADER_DIRECTORIES(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})

# Sources
file(GLOB_RECURSE SOURCES "*.h" "*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE gtest gmock pthread Qt5::Test kjarni)

# Tests
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
