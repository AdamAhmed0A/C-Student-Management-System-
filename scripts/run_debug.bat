@echo off
echo ========================================
echo DEBUG LOG
echo ========================================
echo.
echo [1] Checking for UniManage.exe...
if exist "x64\Debug\UniManage.exe" (
    echo Found in x64\Debug
    set EXE_PATH="x64\Debug\UniManage.exe"
) else (
    echo NOT FOUND in x64\Debug
    echo Checking other locations...
    if exist "UniManage\release\UniManage.exe" set EXE_PATH="UniManage\release\UniManage.exe"
    if exist "UniManage\debug\UniManage.exe" set EXE_PATH="UniManage\debug\UniManage.exe"
)

if not defined EXE_PATH (
    echo ERROR: Could not find UniManage.exe
    pause
    exit /b
)

echo [2] Running %EXE_PATH%...
echo output will be captured below:
echo ----------------------------------------
%EXE_PATH%
echo ----------------------------------------
echo.
echo [3] MySQL Service Status:
sc query MySQL80
sc query MySQL
echo.
pause
