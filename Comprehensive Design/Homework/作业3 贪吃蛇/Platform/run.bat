@echo off
setlocal EnableDelayedExpansion

echo --- Creating build directory if it doesn't exist ---
if not exist build mkdir build

echo --- Compiling C++ Engine ---
g++ -std=c++17 -O2 -Wall engine\main.cpp -o build\engine.exe
if %errorlevel% neq 0 ( echo. && echo !!! Engine Compilation FAILED. && goto end )

echo --- Dynamically Compiling All Bots in 'bots' folder ---
set "BOT_LIST_ARGS="
set COMPILE_ERROR=0

rem 遍历 bots 文件夹下的所有 .cpp 文件
FOR %%F IN (bots\*.cpp) DO (
    echo.
    echo ---------------------------------
    echo Found bot file: %%~nxF

    rem --- 核心逻辑：从文件名解析ID (文件名必须是 ID.cpp) ---
    set "BOT_ID=%%~nF"

    echo  - Parsed ID from filename: !BOT_ID!
    echo  - Compiling...
    g++ -std=c++17 -O2 -Wall "%%F" -o "build\%%~nF.exe" && (
        rem 编译成功后，将从文件名解析出的ID和exe路径添加到参数列表中
        set "BOT_LIST_ARGS=!BOT_LIST_ARGS! !BOT_ID! build\%%~nF.exe"
        echo  - Compilation successful.
    ) || (
        rem 如果编译失败，设置错误标志
        set COMPILE_ERROR=1
        echo  - !!! Compilation FAILED for %%~nxF
    )
    echo ---------------------------------
)

if %COMPILE_ERROR% equ 1 ( echo. && echo !!! One or more bots FAILED to compile. Aborting. && goto end )
if "!BOT_LIST_ARGS!"=="" ( echo. && echo !!! No bots found in 'bots' folder. && goto end )

echo.
echo --- Running Game Simulation (All Bots Battle Royale!) ---
echo Final command-line arguments:%BOT_LIST_ARGS%

rem 使用构建好的参数列表启动游戏
build\engine.exe%BOT_LIST_ARGS% > game.log 2> game.err
if %errorlevel% neq 0 ( echo. && echo !!! Game Simulation FAILED. Check game.err. && goto end )

echo Simulation finished. See game.log and game.err for details.
echo --- Launching Pygame Visualizer ---
"D:\Data\conda_envs\myenv\python.exe" visualizer\visualizer.py game.log

:end
echo.
pause