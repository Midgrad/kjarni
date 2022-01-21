# Project
project(kjarni)

# CMake modules
include(cmake/RecurseSubdirs.cmake)

# Find Qt libraries
find_package(Qt5 ${QT_REQUIRED_VERSION} COMPONENTS Core Sql REQUIRED)

# Target
add_library(${PROJECT_NAME} SHARED "")

# Include
HEADER_DIRECTORIES(INCLUDES "include")
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES} PRIVATE "src")

# Sources
file(GLOB_RECURSE SOURCES "include/*" "src/*")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

# Link with libraries
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Sql loodsman)

# Tests
if (TESTS_ENABLED)
    add_subdirectory(tests)
endif (TESTS_ENABLED)
