# Library Management System in C++

A comprehensive Library Management System implemented in C++ with a MySQL backend. This project demonstrates connecting a C++ application to a MySQL database using MySQL Connector/C++ 9.3.0, with CMake for builds.

---

## Features & Project Scope

- **User Registration & Login:**  
  Secure registration and authentication for members.

- **Role-Based Dashboard Access:**  
  - **Admin & Librarian:**
    - Add, update, and delete book records
    - Manage borrowing and returning of books
    - Search and filter books
    - Manage all user accounts (add, update, delete users, assign roles)
  - **Users:**
    - View and search available books
    - View their own borrowed books

- **Activity Tracking:**  
  - Track issued and returned books per user
  - Calculate and display fines for overdue returns (if implemented)

- **User Role Management:**  
  - Assign admin, librarian, or user roles to each account
  - Dashboard adapts to role-based permissions

---

## Technologies Used

- **C++** (Core application logic)
- **MySQL** (Relational database)
- **MySQL Connector/C++ 9.3.0** (C++ to MySQL connectivity)
- **CMake** (Build system)
- **Git** (Version control)

---

## Setup Instructions

### 1. Clone the Repository

```sh
git clone https://github.com/devhimal/library-management-system-in-cpp.git
cd library-management-system-in-cpp
```

### 2. Install MySQL Server

- Download and install MySQL Server: https://dev.mysql.com/downloads/mysql/
- Start the MySQL service and create a database/user as needed.

### 3. Install MySQL Connector/C++ 9.3.0

- Download: https://dev.mysql.com/downloads/connector/cpp/
- Extract and note the include and library directories.

### 4. Configure the Database

- Create the library database:
  ```sql
  CREATE DATABASE library_db;
  ```
- Create the necessary tables for users, books, and borrowing records (see provided SQL scripts if available).

### 5. Update Database Credentials

- In your C++ source code, update the connection parameters to match your MySQL server setup.

### 6. Configure CMakeLists.txt

Ensure the following lines are included:

```cmake
find_package(MySQL REQUIRED)
include_directories(${MYSQL_INCLUDE_DIRS} /path/to/mysql-connector/include)
link_directories(/path/to/mysql-connector/lib)

add_executable(library main.cpp ... )
target_link_libraries(library ${MYSQL_LIBRARIES} mysqlcppconn)
```

---

## Build & Run

```sh
mkdir build
cd build
cmake ..
make
./libraryManagement
```

---

## Version Control

This project uses Git. Please use branches and clear commit messages. Contribute via pull requests.

---

## Contribution

1. Fork the repo and create a new branch.
2. Make your changes.
3. Push and open a pull request.

---

## License

This project is licensed under the MIT License.

---

> **Note:** Always update your database credentials and ensure the MySQL service is running before launching the application.
