# Linux Run Guide

This guide provides instructions on how to build and run the project on Linux.

## Prerequisites

- GCC 9 or later.
- CMake 3.16 or later.
- Qt 6.2 or later (including `libqt6sql-mysql`).
- MySQL Server 8.0 or later.

## 1. Configure the Environment

1.  Create a `.env` file in the root directory of the project. You can copy the `.env.example` file as a template.
2.  Set the following variables in the `.env` file:
    ```
    DB_HOST=127.0.0.1
    DB_PORT=3306
    DB_USER=<your_mysql_user>
    DB_PASSWORD=<your_mysql_password>
    DB_NAME=<your_mysql_database>
    ```

## 2. Build the Project

1.  Open a terminal and navigate to the root directory of the project.
2.  Create a build directory:
    ```bash
    mkdir build
    ```
3.  Navigate into the build directory:
    ```bash
    cd build
    ```
4.  Run CMake to generate the Makefiles:
    ```bash
    cmake ..
    ```
5.  Run Make to build the project:
    ```bash
    make
    ```

## 3. Run the Application

### GUI Application

To run the GUI application, you need to have an X server running.

```bash
./UniManage/gui/unimanage_gui
```

### CLI Application

```bash
./UniManageCli/unimanage_cli
```

## Database Reset

You can reset the database by executing the `reset_database.sql` script:

```bash
mysql -u <your_mysql_user> -p <your_mysql_database> < ../reset_database.sql
```