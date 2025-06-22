#include "library.h"
#include "connection.h"
#include <iostream>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

Library::Library(const std::string &user, const std::string &role)
    : currentUser(user), currentRole(role) {}

void Library::addBook()
{
    if (currentRole != "admin" && currentRole != "librarian")
    {
        std::cout << "❌ Only admin or librarian can add books.\n";
        return;
    }

    std::string name, description, category, author, publisher;
    double price;
    int year;
    bool available;

    std::cin.ignore();
    std::cout << "Enter book name: ";
    std::getline(std::cin, name);
    std::cout << "Enter description: ";
    std::getline(std::cin, description);
    std::cout << "Enter price: ";
    std::cin >> price;
    std::cin.ignore();
    std::cout << "Enter category: ";
    std::getline(std::cin, category);
    std::cout << "Enter author: ";
    std::getline(std::cin, author);
    std::cout << "Enter publisher: ";
    std::getline(std::cin, publisher);
    std::cout << "Enter published year: ";
    std::cin >> year;
    std::cout << "Is available? (1 = Yes, 0 = No): ";
    std::cin >> available;

    sql::Connection *con = connectDB();
    if (!con)
        return;

    try
    {
        sql::PreparedStatement *stmt = con->prepareStatement(
            "INSERT INTO books(name, description, price, category, author, publisher, year, available) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        stmt->setString(1, name);
        stmt->setString(2, description);
        stmt->setDouble(3, price);
        stmt->setString(4, category);
        stmt->setString(5, author);
        stmt->setString(6, publisher);
        stmt->setInt(7, year);
        stmt->setBoolean(8, available);
        stmt->execute();

        std::cout << "✅ Book added successfully.\n";

        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "❌ Error adding book: " << e.what() << std::endl;
        delete con;
    }
}

void Library::updateBook()
{
    if (currentRole != "admin" && currentRole != "librarian")
    {
        std::cout << "❌ Only admin or librarian can update books.\n";
        return;
    }

    int bookId;
    std::cout << "Enter book ID to update: ";
    std::cin >> bookId;
    std::cin.ignore();

    std::string name, description, category, author, publisher;
    double price;
    int year;
    bool available;

    std::cout << "New name: ";
    std::getline(std::cin, name);
    std::cout << "New description: ";
    std::getline(std::cin, description);
    std::cout << "New price: ";
    std::cin >> price;
    std::cin.ignore();
    std::cout << "New category: ";
    std::getline(std::cin, category);
    std::cout << "New author: ";
    std::getline(std::cin, author);
    std::cout << "New publisher: ";
    std::getline(std::cin, publisher);
    std::cout << "New year: ";
    std::cin >> year;
    std::cout << "Is available? (1 = Yes, 0 = No): ";
    std::cin >> available;

    sql::Connection *con = connectDB();
    if (!con)
        return;

    try
    {
        sql::PreparedStatement *stmt = con->prepareStatement(
            "UPDATE books SET name=?, description=?, price=?, category=?, author=?, publisher=?, year=?, available=? WHERE id=?");
        stmt->setString(1, name);
        stmt->setString(2, description);
        stmt->setDouble(3, price);
        stmt->setString(4, category);
        stmt->setString(5, author);
        stmt->setString(6, publisher);
        stmt->setInt(7, year);
        stmt->setBoolean(8, available);
        stmt->setInt(9, bookId);

        stmt->execute();

        std::cout << "✅ Book updated successfully.\n";

        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "❌ Error updating book: " << e.what() << std::endl;
        delete con;
    }
}

void Library::deleteBook()
{
    if (currentRole != "admin" && currentRole != "librarian")
    {
        std::cout << "❌ Only admin or librarian can delete books.\n";
        return;
    }

    int bookId;
    std::cout << "Enter book ID to delete: ";
    std::cin >> bookId;

    sql::Connection *con = connectDB();
    if (!con)
        return;

    try
    {
        sql::PreparedStatement *stmt = con->prepareStatement("DELETE FROM books WHERE id=?");
        stmt->setInt(1, bookId);
        stmt->execute();

        std::cout << "✅ Book deleted successfully.\n";

        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "❌ Error deleting book: " << e.what() << std::endl;
        delete con;
    }
}

void Library::displayBooks()
{
    sql::Connection *con = connectDB();
    if (!con)
        return;

    try
    {
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT * FROM books");

        std::cout << "\nAvailable Books:\n";
        while (res->next())
        {
            std::cout << "\nID: " << res->getInt("id")
                      << "\nName: " << res->getString("name")
                      << "\nDescription: " << res->getString("description")
                      << "\nPrice: $" << res->getDouble("price")
                      << "\nCategory: " << res->getString("category")
                      << "\nAuthor: " << res->getString("author")
                      << "\nPublisher: " << res->getString("publisher")
                      << "\nYear: " << res->getInt("year")
                      << "\nAvailable: " << (res->getBoolean("available") ? "Yes" : "No")
                      << "\n-----------------------------";
        }

        delete res;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "❌ Error displaying books: " << e.what() << std::endl;
        delete con;
    }
}

void Library::searchBook()
{
    std::string keyword;
    std::cout << "Enter book name, author or category to search: ";
    std::cin.ignore();
    std::getline(std::cin, keyword);

    sql::Connection *con = connectDB();
    if (!con)
        return;

    try
    {
        sql::PreparedStatement *stmt = con->prepareStatement(
            "SELECT * FROM books WHERE name LIKE ? OR author LIKE ? OR category LIKE ?");
        std::string kw = "%" + keyword + "%";
        stmt->setString(1, kw);
        stmt->setString(2, kw);
        stmt->setString(3, kw);

        sql::ResultSet *res = stmt->executeQuery();

        std::cout << "\nSearch results:\n";
        while (res->next())
        {
            std::cout << "\nID: " << res->getInt("id")
                      << "\nName: " << res->getString("name")
                      << "\nAuthor: " << res->getString("author")
                      << "\nCategory: " << res->getString("category")
                      << "\nYear: " << res->getInt("year")
                      << "\nAvailable: " << (res->getBoolean("available") ? "Yes" : "No")
                      << "\n-----------------------------";
        }

        delete res;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "❌ Error searching books: " << e.what() << std::endl;
        delete con;
    }
}
