@echo off
echo ========================================
echo FORCE FIX - Visual Studio Build
echo ========================================
echo.

REM Close EVERYTHING
echo Killing all processes...
taskkill /F /IM UniManage.exe 2>nul
taskkill /F /IM devenv.exe 2>nul
taskkill /F /IM MSBuild.exe 2>nul
taskkill /F /IM VBCSCompiler.exe 2>nul
taskkill /F /IM qmake.exe 2>nul
taskkill /F /IM mingw32-make.exe 2>nul
timeout /t 2 /nobreak >nul

REM Delete EVERYTHING
echo Deleting all build files...
cd C:\Users\Adam\C-Student-Management-System-
rmdir /S /Q x64 2>nul
rmdir /S /Q .vs 2>nul
rmdir /S /Q Debug 2>nul
rmdir /S /Q Release 2>nul
rmdir /S /Q UniManage\x64 2>nul
rmdir /S /Q UniManage\Debug 2>nul
rmdir /S /Q UniManage\Release 2>nul
rmdir /S /Q build 2>nul
del /S /Q *.obj 2>nul
del /S /Q *.exe 2>nul
timeout /t 2 /nobreak >nul

echo.
echo ========================================
echo DONE! All files deleted.
echo ========================================
echo.
echo NOW DO THIS:
echo.
echo 1. Right-click Visual Studio icon
echo 2. Click "Run as Administrator"
echo 3. Open your solution
echo 4. Build -^> Rebuild Solution
echo 5. Press Ctrl+F5 to run
echo.
echo ========================================
pause
