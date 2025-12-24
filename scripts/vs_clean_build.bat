@echo off
echo ========================================
echo Visual Studio Clean Build Script
echo ========================================
echo.
echo This will:
echo - Close all running processes
echo - Delete all build folders
echo - Remove locked files
echo.
pause

echo.
echo [1/5] Closing all UniManage processes...
taskkill /F /IM UniManage.exe 2>nul
taskkill /F /IM devenv.exe 2>nul
taskkill /F /IM MSBuild.exe 2>nul
taskkill /F /IM VBCSCompiler.exe 2>nul
timeout /t 3 /nobreak >nul

echo [2/5] Deleting build folders...
cd C:\Users\Adam\C-Student-Management-System-
rmdir /S /Q x64 2>nul
rmdir /S /Q .vs 2>nul
rmdir /S /Q Debug 2>nul
rmdir /S /Q Release 2>nul
rmdir /S /Q UniManage\x64 2>nul
rmdir /S /Q UniManage\Debug 2>nul
rmdir /S /Q UniManage\Release 2>nul

echo [3/5] Deleting object files...
del /S /Q *.obj 2>nul
del /S /Q *.pch 2>nul
del /S /Q *.ilk 2>nul
del /S /Q *.pdb 2>nul

echo [4/5] Deleting any locked exe files...
del /F /Q "x64\Debug\UniManage.exe" 2>nul
del /F /Q "x64\Release\UniManage.exe" 2>nul
del /F /Q "Debug\UniManage.exe" 2>nul
del /F /Q "Release\UniManage.exe" 2>nul

echo [5/5] Waiting for file system to release locks...
timeout /t 3 /nobreak >nul

echo.
echo ========================================
echo CLEANUP COMPLETE!
echo ========================================
echo.
echo Next steps:
echo 1. Open Visual Studio AS ADMINISTRATOR
echo    (Right-click Visual Studio -^> Run as Administrator)
echo.
echo 2. Open your solution file
echo.
echo 3. Build -^> Rebuild Solution
echo.
echo 4. Press Ctrl+F5 (Start Without Debugging)
echo    DO NOT press F5 (Start Debugging)
echo.
echo ========================================
pause
