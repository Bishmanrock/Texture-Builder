@echo off
setlocal

:: Define the output filenames as variables
set PACKAGER_OUTPUT=TexturePackager.exe
set READER_OUTPUT=TextureReader.exe

:: Define the error log file
set ERROR_LOG=error_log.txt

:: Compile the TexturePackager
g++ TextureCompiler.cpp TexturePackager.cpp DirectoryReader.cpp -o %PACKAGER_OUTPUT% -Iincludes 2>error_log.txt

:: Check if TexturePackager compilation was successful
if %errorlevel% neq 0 (
    echo TexturePackager compilation failed! Check %ERROR_LOG% for details.
    type error_log.txt
) else (
    echo TexturePackager compilation successful!
)

:: Compile the TextureReader
g++ TextureReader.cpp -o %READER_OUTPUT% -Iincludes 2>error_log.txt

:: Check if TextureReader compilation was successful
if %errorlevel% neq 0 (
    echo TextureReader compilation failed! Check %ERROR_LOG% for details.
    type error_log.txt
) else (
    echo TextureReader compilation successful!
)

:: Pause to keep the console window open
pause
