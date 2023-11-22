@echo off

set ROOT_DIR=%~dp0
set BUILD_DIR=%ROOT_DIR%build

@REM rem 检查是否存在 build 文件夹
@REM if exist "%BUILD_DIR%" (
@REM     echo %BUILD_DIR%
@REM     rmdir /s /q "%BUILD_DIR%"
@REM )

mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

cmake -A x64 ^
-DCMAKE_BUILD_TYPE=Release ^
-DCMAKE_SYSTEM_NAME=Windows ^
-DINTTYPES_FORMAT=C99 ^
..

cmake --build . --config Release -j8

cd %BUILD_DIR%\Release
"PlugInUI.exe"
cd ..\..