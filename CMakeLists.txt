# CMakeLists.txt (root CMake file)

# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.16)

# Set the project name and supported languages
project("MapGeneratorTool" LANGUAGES CXX)

# Define architecture and configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "Available build configurations")
set(CMAKE_GENERATOR_PLATFORM "x64")

# Set the default startup project (used by some IDEs, like Visual Studio)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "App")

# Set output directories based on the configuration and architecture
set(OutputDir "${CMAKE_SYSTEM_NAME}-${CMAKE_GENERATOR_PLATFORM}/$<CONFIG>")

# Add MSVC-specific compiler options
if(MSVC)
    add_compile_options("/EHsc" "/Zc:preprocessor" "/Zc:__cplusplus")
endif()

# Group and include projects
# Core Group
add_subdirectory("Core")
add_subdirectory("MapGenerator")
add_subdirectory("GUI")

# App Group
add_subdirectory("App")

