#include "connection.h"
#include <iostream>

sql::Connection *connectDB()
{
    try
    {
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con = driver->connect("tcp://127.0.0.1:3306", "admin_user", "StrongPass123");
        con->setSchema("library_db");
        return con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error connecting to DB: " << e.what() << std::endl;
        return nullptr;
    }
}
