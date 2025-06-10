#include <iostream>
#include <string>
#include <limits>
#include "library.h"
#include "connection.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

// Function to authenticate user and return role
bool authenticateUser(std::string &username, std::string &role)
{
    std::string password;
    std::cout << "👤 Enter username: ";
    std::cin >> username;
    std::cout << "🔐 Enter password: ";
    std::cin >> password;

    sql::Connection *con = connectDB();
    if (!con)
        return false;

    try
    {
        sql::PreparedStatement *stmt = con->prepareStatement(
            "SELECT role FROM users WHERE username = ? AND pass = ?");
        stmt->setString(1, username);
        stmt->setString(2, password);

        sql::ResultSet *res = stmt->executeQuery();

        if (res->next())
        {
            role = res->getString("role");
            delete res;
            delete stmt;
            delete con;
            return true;
        }
        else
        {
            std::cout << "❌ Invalid credentials.\n";
        }

        delete res;
        delete stmt;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "❌ Login error: " << e.what() << std::endl;
    }

    delete con;
    return false;
}

// Clear input stream after invalid input
void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
    std::string username, role;

    std::cout << "=== 📚 Library Management System ===\n";

    if (!authenticateUser(username, role))
    {
        return 1;
    }

    Library library(username, role);
    int choice;

    do
    {
        std::cout << "\nWelcome, " << username << " (" << role << ")\n";
        std::cout << "====== Menu ======\n";
        std::cout << "1. Display Books\n";
        std::cout << "2. Search Book\n";

        if (role == "admin")
        {
            std::cout << "3. Add Book\n";
            std::cout << "4. Delete Book\n";
            std::cout << "5. Update Book\n";
        }

        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (std::cin.fail())
        {
            clearInput();
            std::cout << "❗ Invalid input.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            library.displayBooks();
            break;
        case 2:
            library.searchBook();
            break;
        case 3:
            if (role == "admin")
                library.addBook();
            else
                std::cout << "❌ Access denied.\n";
            break;
        case 4:
            if (role == "admin")
                library.deleteBook();
            else
                std::cout << "❌ Access denied.\n";
            break;
        case 5:
            if (role == "admin")
                library.updateBook();
            else
                std::cout << "❌ Access denied.\n";
            break;
        case 0:
            std::cout << "👋 Exiting...\n";
            break;
        default:
            std::cout << "❗ Invalid option.\n";
        }

    } while (choice != 0);

    return 0;
}
