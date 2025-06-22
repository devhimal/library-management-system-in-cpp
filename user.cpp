#include "user.h"
#include "connection.h"
#include <iostream>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

bool User::registerUser()
{
    std::string pass, roleInput;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter pass: ";
    std::cin >> pass;

    // Role selection for admin/librarian must be done by admin manually; users default role "user"
    role = "user";

    sql::Connection *con = connectDB();
    if (!con)
        return false;

    try
    {
        // Check if username exists
        sql::PreparedStatement *checkStmt = con->prepareStatement("SELECT username FROM users WHERE username=?");
        checkStmt->setString(1, username);
        sql::ResultSet *res = checkStmt->executeQuery();
        if (res->next())
        {
            std::cout << "Username already exists. Try a different one.\n";
            delete res;
            delete checkStmt;
            delete con;
            return false;
        }
        delete res;
        delete checkStmt;

        // Insert user with default "user" role
        sql::PreparedStatement *stmt = con->prepareStatement("INSERT INTO users(username, pass, role) VALUES (?, ?, ?)");
        stmt->setString(1, username);
        stmt->setString(2, pass);
        stmt->setString(3, role);
        stmt->execute();

        delete stmt;
        delete con;
        std::cout << "Registration successful. You are assigned the 'user' role.\n";
        return true;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Registration error: " << e.what() << std::endl;
        delete con;
        return false;
    }
}

bool User::loginUser()
{
    std::string pass;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "pass: ";
    std::cin >> pass;

    sql::Connection *con = connectDB();
    if (!con)
        return false;

    try
    {
        sql::PreparedStatement *stmt = con->prepareStatement("SELECT role FROM users WHERE username=? AND pass=?");
        stmt->setString(1, username);
        stmt->setString(2, pass);
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
            std::cout << "❌ Login failed. Invalid username or pass.\n";
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

std::string User::getUsername() const { return username; }
std::string User::getRole() const { return role; }
