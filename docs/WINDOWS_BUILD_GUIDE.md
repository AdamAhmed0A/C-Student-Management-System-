# Windows Build and Run Guide

This guide provides instructions on how to build and run the project on Windows.

## Prerequisites

- Visual Studio 2019 or later with the "Desktop development with C++" workload installed.
- CMake 3.16 or later.
- Qt 6.2 or later.
- MySQL Server 8.0 or later.

## 1. Configure the Database

1.  Open `scripts/configure_db.bat` in a text editor.
2.  Set the `DB_USER`, `DB_PASS`, and `DB_NAME` variables to your MySQL database credentials.
3.  Run the script by double-clicking it or running it from the command line.

## 2. Build the Project

1.  Open a command prompt and navigate to the root directory of the project.
2.  Run the `scripts/build.bat` script. This will create a `build` directory and generate the Visual Studio solution files.

## 3. Run the Application

### GUI Application

1.  Open the `build/SIS-Project.sln` file in Visual Studio.
2.  Set the `unimanage_gui` project as the startup project.
3.  Press `F5` to build and run the application.

### CLI Application

1.  Open the `build/SIS-Project.sln` file in Visual Studio.
2.  Set the `unimanage_cli` project as the startup project.
3.  Press `F5` to build and run the application.

## Other Scripts

-   `scripts/reset_db.bat`: Resets the database by dropping and recreating the tables.
-   `scripts/vs_clean_build.bat`: Deletes the `build` directory to perform a clean build.
-   `scripts/FORCE_FIX.bat`: Attempts to fix common build issues.
-   `scripts/run_debug.bat`: Runs the `unimanage_gui` application in debug mode.