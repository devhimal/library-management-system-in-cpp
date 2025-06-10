#include "user.h"
#include "connection.h"
#include <iostream>
#include <string>

bool User::login()
{
    std::string password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    sql::Connection *con = connectDB();
    if (!con)
        return false;

    try
    {
        sql::PreparedStatement *stmt = con->prepareStatement("SELECT role FROM users WHERE username=? AND password=?");
        stmt->setString(1, username);
        stmt->setString(2, password);
        sql::ResultSet *res = stmt->executeQuery();

        if (res->next())
        {
            role = res->getString("role");
            std::cout << "Login successful. Role: " << role << std::endl;
            delete res;
            delete stmt;
            delete con;
            return true;
        }
        else
        {
            std::cout << "❌ Login failed.\n";
        }
        delete res;
        delete stmt;
        delete con;
    }
    catch (...)
    {
        std::cerr << "Login query failed.\n";
    }
    return false;
}

std::string User::getRole()
{
    return role;
}
