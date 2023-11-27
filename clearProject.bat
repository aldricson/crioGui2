@echo off
echo Clearing the project...

REM 1) Delete all files containing "ninja" in the file name
for %%f in (*ninja*) do (
    echo Deleting: %%f
    del "%%f"
)

REM 2) Delete all files containing ".exe" in the name or extension
for %%f in (*.*exe*) do (
    echo Deleting: %%f
    del "%%f"
)

REM 3) Delete cmake_install.cmake
if exist cmake_install.cmake (
    echo Deleting: cmake_install.cmake
    del cmake_install.cmake
)

REM 4) Delete CMakeCache.txt
if exist CMakeCache.txt (
    echo Deleting: CMakeCache.txt
    del CMakeCache.txt
)

REM 5) Delete qtcsettings.cmake
if exist qtcsettings.cmake (
    echo Deleting: qtcsettings.cmake
    del qtcsettings.cmake
)

REM 6) Delete the CMakeFiles folder and its content
if exist CMakeFiles (
    echo Deleting CMakeFiles folder and its content...
    rmdir /s /q CMakeFiles
)

REM 7) Delete every folder containing "_autogen" in the folder name with all its content
for /d %%d in (*_autogen*) do (
    echo Deleting folder: %%d
    rmdir /s /q "%%d"
)

REM 8) Delete the .qt folder and its content
if exist .qt (
    echo Deleting .qt folder and its content...
    rmdir /s /q .qt
)

REM 9) Delete the .qtc_clangd folder and its content
if exist .qtc_clangd (
    echo Deleting .qtc_clangd folder and its content...
    rmdir /s /q .qtc_clangd
)

REM 10) Delete the Testing folder and its content
if exist Testing (
    echo Deleting Testing folder and its content...
    rmdir /s /q Testing
)

REM 11) Delete the .cmake folder and its content
if exist .cmake (
    echo Deleting .cmake folder and its content...
    rmdir /s /q .cmake
)

echo Project cleanup complete.
