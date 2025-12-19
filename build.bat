@echo off
echo ========================================
echo Building UniManage Application
echo ========================================

cd UniManage

echo.
echo Step 1: Running qmake...
call qmake

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: qmake failed!
    echo Please ensure Qt is installed and qmake is in your PATH
    pause
    exit /b 1
)

echo.
echo Step 2: Compiling with mingw32-make...
call mingw32-make

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo You can now run the application from:
echo %CD%\release\UniManage.exe
echo or
echo %CD%\debug\UniManage.exe
echo.
pause
