cmake_minimum_required(VERSION 3.18)
project(aligncount_cpp LANGUAGES CXX)

# Delegate to your standalone folder
add_subdirectory(cpp/standalone standalone_build)

# Ensure the CLI gets installed into the wheel
install(TARGETS AligncountStandalone RUNTIME DESTINATION bin)