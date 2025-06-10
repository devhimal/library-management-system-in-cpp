cmake_minimum_required(VERSION 3.10)
project(LibraryManagement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory for MySQL Connector/C++ headers
include_directories(/usr/local/mysql-connector-c++-9.3.0/include/jdbc)

# Link directory for MySQL Connector/C++ library
link_directories(/usr/local/mysql-connector-c++-9.3.0/lib64)

# Add your project source files
add_executable(LibraryManagement
    main.cpp
    connection.cpp
    library.cpp
)

# Link against the MySQL Connector/C++ library
target_link_libraries(LibraryManagement mysqlcppconn)
