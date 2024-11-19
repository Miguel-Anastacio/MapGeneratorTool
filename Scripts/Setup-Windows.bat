@echo off

:: Change to the parent directory
pushd ..

:: Create a build directory if it doesn't exist
if not exist build (
    mkdir build
)

:: Navigate to the build directory
cd build

:: Run CMake to generate Visual Studio 2022 project files
cmake -G "Visual Studio 17 2022" -A x64 ..

:: Return to the original directory
popd

:: Pause to show any output or errors
pause