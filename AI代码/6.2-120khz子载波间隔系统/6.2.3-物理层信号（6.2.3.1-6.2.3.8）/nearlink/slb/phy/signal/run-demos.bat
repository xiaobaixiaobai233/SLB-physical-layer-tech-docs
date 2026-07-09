@echo off
chcp 65001 >nul
setlocal

set "ROOT=%~dp0"
set "CMAKE=D:\Microsoft Visual Studio\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

if exist "%CMAKE%" (
    "%CMAKE%" --build "%ROOT%build" --config Debug
    if errorlevel 1 exit /b 1
) else (
    echo cmake not found. Build in Cursor/VS first, or set CMAKE path in this script.
)

echo.
echo === Running demos ===
"%ROOT%build\Debug\demo_6_2_3_1_sync.exe"
"%ROOT%build\Debug\demo_6_2_3_2_3_ref.exe"
"%ROOT%build\Debug\demo_6_2_3_4_7_dmrs.exe"
"%ROOT%build\Debug\demo_6_2_3_8_pas.exe"

endlocal
