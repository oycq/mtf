@echo off
setlocal

REM 设置项目根目录
set PROJECT_DIR=%~dp0

REM 设置生成目录
set BUILD_DIR=%PROJECT_DIR%build

REM 创建生成目录
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

REM 进入生成目录
cd %BUILD_DIR%

REM 运行 CMake 以生成 Visual Studio 2019 项目文件
cmake -G "Visual Studio 16 2019" -A x64 %PROJECT_DIR%

REM 检查 CMake 配置是否成功
if errorlevel 1 (
    echo CMake configuration failed.
    endlocal
    pause
    exit /b 1
)

REM 构建项目
cmake --build . --config Release

REM 检查构建是否成功
if errorlevel 1 (
    echo Build failed.
    endlocal
    pause
    exit /b 1
)

REM 返回项目根目录
cd %PROJECT_DIR%

REM 执行生成的可执行文件
if exist %BUILD_DIR%\Release\mtf.exe (
    echo Running mtf.exe...
    %BUILD_DIR%\Release\mtf.exe
) else (
    echo Build succeeded but mtf.exe not found.
)

py scripts/show_c_result.py

endlocal
REM pause
