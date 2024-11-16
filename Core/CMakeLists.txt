# Core/CMakeLists.txt

# Specify that Core is a static library
add_library(Core STATIC)

# Set the language and C++ standard
set_target_properties(Core PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Specify the output directories for the compiled library and object files
set(OutputDir "${CMAKE_SYSTEM_NAME}-${CMAKE_GENERATOR_PLATFORM}/$<CONFIG>")
set_target_properties(Core PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${OutputDir}/Core"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${OutputDir}/Core"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${OutputDir}/Core"
    OBJECT_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/Intermediates/${OutputDir}/Core"
)

# Include directories for the Core project
target_include_directories(Core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_include_directories(Core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty")

# Specify the source files
file(GLOB_RECURSE CORE_SOURCES "Source/*.cpp" "Source/*.h" "Source/*.hpp")
# file(GLOB_RECURSE JSON_SOURCES "thirdParty/*.hpp")
target_sources(Core PRIVATE ${CORE_SOURCES})
# target_sources(Core PRIVATE ${JSON_SOURCES})

# Organize files by their folder structure in the Visual Studio solution
source_group(TREE ${CMAKE_SOURCE_DIR}/Core/source PREFIX "Source Files" FILES ${CORE_SOURCES}) 
# source_group(TREE ${CMAKE_SOURCE_DIR}/Core/thirdParty PREFIX "Thirdparty" FILES ${JSON_SOURCES}) 

# Disable static runtime linking (equivalent to `staticruntime "off"`)
# This can be done with specific flags for MSVC
if(MSVC)
target_compile_options(Core PUBLIC "/MD$<$<CONFIG:Debug>:d>")
endif()

# Platform-specific settings for Windows
if(WIN32)
target_compile_definitions(Core PRIVATE "_WINDOWS")
set_target_properties(Core PROPERTIES
MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"  # Matches /MD for Release, /MDd for Debug
    )
endif()
    

# Configuration-specific settings
# Debug configuration
target_compile_definitions(Core PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)
target_compile_options(Core PRIVATE
    $<$<CONFIG:Debug>:-g>
)
set_target_properties(Core PROPERTIES
    DEBUG_POSTFIX "d"
)

# Release configuration
target_compile_definitions(Core PRIVATE
    $<$<CONFIG:Release>:RELEASE>
)
set_target_properties(Core PROPERTIES
    OPTIMIZE_POSTFIX "opt"
)

# Distribution configuration
target_compile_definitions(Core PRIVATE
    $<$<CONFIG:Dist>:DIST>
)
target_compile_options(Core PRIVATE
    $<$<CONFIG:Dist>:-O3>
    $<$<CONFIG:Dist>:-DNDEBUG>
)
set_target_properties(Core PROPERTIES
    DIST_POSTFIX "dist"
)


