# App/CMakeLists.txt

# Define the App as a console application
add_executable(App)

# Set the language and C++ standard
set_target_properties(App PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Specify the output directories for the executable and object files
set(OutputDir "${CMAKE_SYSTEM_NAME}-${CMAKE_GENERATOR_PLATFORM}/$<CONFIG>")
set_target_properties(App PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${OutputDir}/App"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${OutputDir}/App"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/${OutputDir}/App"
    OBJECT_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries/Intermediates/${OutputDir}/App"
)

# Source files for App
# add_subdirectory(thirdparty/)
file(GLOB_RECURSE APP_SOURCES "Source/*.cpp" "Source/*.h")
file(GLOB_RECURSE LODEPNG_SOURCES "thirdparty/*.cpp" "thirdparty/*.h")
target_sources(App PRIVATE ${APP_SOURCES})
target_sources(App PRIVATE ${LODEPNG_SOURCES})

source_group(TREE ${CMAKE_SOURCE_DIR}/App/source PREFIX "Source Files" FILES ${APP_SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR}/App/thirdparty PREFIX "Thirdparty" FILES ${LODEPNG_SOURCES})

# Include directories for App and Core and MapGenerator
target_include_directories(App PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_SOURCE_DIR}/Core/Source"
    "${CMAKE_SOURCE_DIR}/MapGenerator/Source"
    "${CMAKE_SOURCE_DIR}/GUI/Source"
)

# Link with Core library
target_link_libraries(App PRIVATE Core)
target_link_libraries(App PRIVATE MapGenerator)
target_link_libraries(App PRIVATE GUI)

# Disable static runtime linking (equivalent to `staticruntime "off"`)
if(MSVC)
    target_compile_options(App PUBLIC "/MD$<$<CONFIG:Debug>:d>")
endif()

# Platform-specific settings for Windows
if(WIN32)
    target_compile_definitions(App PRIVATE "WINDOWS")
    set_target_properties(App PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    )
endif()

# Configuration-specific settings
# Debug configuration
target_compile_definitions(App PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_options(App PRIVATE $<$<CONFIG:Debug>:-g>)
set_target_properties(App PROPERTIES DEBUG_POSTFIX "d")

# Release configuration
target_compile_definitions(App PRIVATE $<$<CONFIG:Release>:RELEASE>)
target_compile_options(App PRIVATE $<$<CONFIG:Release>:-O2>)
set_target_properties(App PROPERTIES OPTIMIZE_POSTFIX "opt")

# Distribution configuration
target_compile_definitions(App PRIVATE $<$<CONFIG:Dist>:DIST>)
target_compile_options(App PRIVATE $<$<CONFIG:Dist>:-O3 -DNDEBUG>)
set_target_properties(App PROPERTIES DIST_POSTFIX "dist")

add_custom_command(TARGET App POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:App> $<TARGET_RUNTIME_DLLS:App>
  COMMAND_EXPAND_LISTS
)
