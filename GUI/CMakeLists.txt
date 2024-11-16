# GUI/CMakeLists.txt

# Specify that GUI is a static library
add_library(GUI STATIC)

# Set the language and C++ standard
set_target_properties(GUI PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Specify the output directories for the compiled library and object files
set(OutputDir "${CMAKE_SYSTEM_NAME}-${CMAKE_GENERATOR_PLATFORM}/$<CONFIG>")
set_target_properties(GUI PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${OutputDir}/GUI"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${OutputDir}/GUI"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${OutputDir}/GUI"
    OBJECT_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/Intermediates/${OutputDir}/GUI"
)

# Include directories for the GUI project
target_include_directories(GUI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# Specify the source files
file(GLOB_RECURSE GUI_SOURCES "Source/*.cpp" "Source/*.h" "Source/*.hpp")
target_sources(GUI PRIVATE ${GUI_SOURCES})

# Organize files by their folder structure in the Visual Studio solution
source_group(TREE ${CMAKE_SOURCE_DIR}/GUI/source PREFIX "Source Files" FILES ${GUI_SOURCES})

# Disable static runtime linking (equivalent to `staticruntime "off"`)
# This can be done with specific flags for MSVC
if(MSVC)
    target_compile_options(GUI PUBLIC "/MD$<$<CONFIG:Debug>:d>")
endif()

# Platform-specific settings for Windows
if(WIN32)
    target_compile_definitions(GUI PRIVATE "_WINDOWS")
    set_target_properties(GUI PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"  # Matches /MD for Release, /MDd for Debug
    )
endif()

add_subdirectory(thirdparty/)
target_include_directories(GUI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/")

# link to core
target_link_libraries(GUI PRIVATE Core)
# Add Libraries
#  GUI
target_link_libraries(GUI PUBLIC ImGui-SFML::ImGui-SFML)


set(SFML_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SFML/bin")
target_compile_definitions(GUI PUBLIC SFML_DLL_DIR="${SFML_DLL_DIR}")

# Configuration-specific settings
# Debug configuration
target_compile_definitions(GUI PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)
target_compile_options(GUI PRIVATE
    $<$<CONFIG:Debug>:-g>
)
set_target_properties(GUI PROPERTIES
    DEBUG_POSTFIX "d"
)

# Release configuration
target_compile_definitions(GUI PRIVATE
    $<$<CONFIG:Release>:RELEASE>
)
set_target_properties(GUI PROPERTIES
    OPTIMIZE_POSTFIX "opt"
)

# Distribution configuration
target_compile_definitions(GUI PRIVATE
    $<$<CONFIG:Dist>:DIST>
)
target_compile_options(GUI PRIVATE
    $<$<CONFIG:Dist>:-O3>
    $<$<CONFIG:Dist>:-DNDEBUG>
)
set_target_properties(GUI PROPERTIES
    DIST_POSTFIX "dist"
)


