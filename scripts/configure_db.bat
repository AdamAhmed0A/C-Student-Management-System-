@echo off
echo ========================================
echo MySQL Configuration Helper
echo ========================================
echo.
echo This will help you configure the database connection.
echo.

:input_host
set /p DB_HOST="Enter MySQL Host (default: 127.0.0.1): "
if "%DB_HOST%"=="" set DB_HOST=127.0.0.1

:input_port
set /p DB_PORT="Enter MySQL Port (default: 3306): "
if "%DB_PORT%"=="" set DB_PORT=3306

:input_user
set /p DB_USER="Enter MySQL Username (default: root): "
if "%DB_USER%"=="" set DB_USER=root

:input_password
set /p DB_PASSWORD="Enter MySQL Password: "
if "%DB_PASSWORD%"=="" (
    echo WARNING: Password is empty!
    set /p confirm="Continue with empty password? (y/n): "
    if /i not "%confirm%"=="y" goto input_password
)

:input_dbname
set /p DB_NAME="Enter Database Name (default: university): "
if "%DB_NAME%"=="" set DB_NAME=university

echo.
echo ========================================
echo Configuration Summary:
echo ========================================
echo Host: %DB_HOST%
echo Port: %DB_PORT%
echo User: %DB_USER%
echo Password: %DB_PASSWORD%
echo Database: %DB_NAME%
echo ========================================
echo.

set /p confirm="Is this correct? (y/n): "
if /i not "%confirm%"=="y" goto input_host

echo.
echo Setting environment variables for this session...
setx DB_HOST "%DB_HOST%"
setx DB_PORT "%DB_PORT%"
setx DB_USER "%DB_USER%"
setx DB_PASSWORD "%DB_PASSWORD%"
setx DB_NAME "%DB_NAME%"

echo.
echo ========================================
echo Configuration saved!
echo ========================================
echo.
echo Environment variables have been set.
echo.
echo IMPORTANT: You need to RESTART your application
echo for the new settings to take effect!
echo.
echo Next steps:
echo 1. Close this window
echo 2. Rebuild the application
echo 3. Run the application
echo.

pause
