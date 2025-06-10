#ifndef CONNECTION_H
#define CONNECTION_H

// MySQL Connector/C++ headers
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

sql::Connection *connectDB();

#endif // CONNECTION_H
