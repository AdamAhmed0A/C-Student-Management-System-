# CLI University Project

This is a Command Line Interface (CLI) version of the UniManage system. It reuses the same business logic (controllers) and database layers as the GUI version but runs in the terminal.

## Features

- **Login System**: Secure login for Admins, Professors, and Students.
- **Admin Dashboard**: Manage students, professors, courses, facilities (split into Halls and Labs), and academic setup.
- **Professor Dashboard**: View courses, managed assigned student grades, and access course lists.
- **Student Dashboard**: View personal grades (hiding unassigned ones) and schedule info.

## How to Build and Run

This project is built using Qt. To build it from the command line:

1. Ensure Qt is in your PATH.
2. Open a terminal in this folder.
3. Run `qmake` to generate the Makefile.
4. Run `make` (or `nmake` for MSVC, `mingw32-make` for MinGW) to build the executable.
5. Run the generated `CLI_University_Project.exe`.

Alternatively, you can open `CLI_Project.pro` in **Qt Creator** and run it as a Console Application.

## Database Connection

The CLI project uses the same database configuration defined in `../../database/dbconnection.cpp`. Ensure your local MySQL/MariaDB server is running before starting the application.
