cmake_minimum_required(VERSION 3.15...3.26)
project(cpp_python_tool_template VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard")

# --- Configure doctest as an external project: ---
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
        doctest
        PREFIX ${CMAKE_BINARY_DIR}/doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        TIMEOUT 10
        UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
)

# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(doctest source_dir)
set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")

# --- Application executable ---
add_executable(linecount src/main.cpp)
install(TARGETS linecount RUNTIME DESTINATION bin)

# --- Test executable ---
# Make test executable
add_executable(test_linecount tests/cpp/test_main.cpp)
target_include_directories(test_linecount PUBLIC ${DOCTEST_INCLUDE_DIR})

# Make test_linecount wait until doctest has finished downloading
add_dependencies(test_linecount doctest)

enable_testing()
add_test(NAME linecount_test COMMAND test_linecount)