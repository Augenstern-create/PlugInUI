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

xcopy /E /I /Y /D %ROOT_DIR%config %BUILD_DIR%\Release
xcopy /E /I /Y /S /D %ROOT_DIR%photograph %BUILD_DIR%\Release\photograph
xcopy /C /I /Y /D %ROOT_DIR%lib\MemProcFS\lib\*.dll "%BUILD_DIR%\Release" 
xcopy /C /I /Y /D %ROOT_DIR%lib\curl\lib\*.dll "%BUILD_DIR%\Release" 


cd %BUILD_DIR%\Release
"UPdate.exe"
cd ..\..