@echo off
echo ========================================
echo Database Reset Script
echo ========================================
echo.
echo This will:
echo - Drop the existing 'university' database
echo - Create a fresh database with correct schema
echo - Add default admin user and test data
echo.
echo WARNING: This will DELETE ALL existing data!
echo.
pause

echo.
echo Enter your MySQL root password when prompted...
echo.

mysql -u root -p < reset_database.sql

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo SUCCESS! Database reset completed.
    echo ========================================
    echo.
    echo Default admin login:
    echo   Username: admin
    echo   Password: admin123
    echo.
    echo Next steps:
    echo 1. Rebuild the application
    echo 2. Run the application
    echo 3. Login with admin credentials
    echo 4. Click "Test Database" button to verify
    echo.
) else (
    echo.
    echo ========================================
    echo ERROR: Database reset failed!
    echo ========================================
    echo.
    echo Please check:
    echo - MySQL is running
    echo - Root password is correct
    echo - You have permission to create databases
    echo.
)

pause
