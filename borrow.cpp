#include "borrow.h"
#include "connection.h"
#include <iostream>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <ctime>

Borrow::Borrow(const std::string &user, const std::string &role)
    : currentUser(user), currentRole(role) {}

void Borrow::borrowBook()
{
    if (currentRole != "admin" && currentRole != "librarian")
    {
        std::cout << "❌ Only admin or librarian can issue books.\n";
        return;
    }

    int bookId, userId;
    std::cout << "Enter book ID to borrow: ";
    std::cin >> bookId;
    std::cout << "Enter user ID who borrows: ";
    std::cin >> userId;

    sql::Connection *con = connectDB();
    if (!con)
        return;

    try
    {
        // Check availability
        sql::PreparedStatement *checkStmt = con->prepareStatement("SELECT available FROM books WHERE id=?");
        checkStmt->setInt(1, bookId);
        sql::ResultSet *res = checkStmt->executeQuery();
        if (!res->next())
        {
            std::cout << "Book not found.\n";
            delete res;
            delete checkStmt;
            delete con;
            return;
        }
        bool available = res->getBoolean("available");
        delete res;
        delete checkStmt;
        if (!available)
        {
            std::cout << "Book not available.\n";
            delete con;
            return;
        }

        // Insert borrowed_books record with borrow_date = now(), returned = 0
        time_t now = time(nullptr);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
        std::string borrowDate(buf);

        sql::PreparedStatement *insertStmt = con->prepareStatement(
            "INSERT INTO borrowed_books(user_id, book_id, borrow_date, returned) VALUES (?, ?, ?, 0)");
        insertStmt->setInt(1, userId);
        insertStmt->setInt(2, bookId);
        insertStmt->setString(3, borrowDate);
        insertStmt->execute();

        // Update book availability to false
        sql::PreparedStatement *updateStmt = con->prepareStatement("UPDATE books SET available=0 WHERE id=?");
        updateStmt->setInt(1, bookId);
        updateStmt->execute();

        std::cout << "✅ Book issued successfully to user ID " << userId << ".\n";

        delete insertStmt;
        delete updateStmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error issuing book: " << e.what() << std::endl;
        delete con;
    }
}

void Borrow::returnBook()
{
    if (currentRole != "admin" && currentRole != "librarian")
    {
        std::cout << "❌ Only admin or librarian can return books.\n";
        return;
    }

    int borrowId;
    std::cout << "Enter borrowed record ID to return: ";
    std::cin >> borrowId;

    sql::Connection *con = connectDB();
    if (!con)
        return;

    try
    {
        // Check borrowed record and if already returned
        sql::PreparedStatement *checkStmt = con->prepareStatement(
            "SELECT book_id, borrow_date, returned FROM borrowed_books WHERE id=?");
        checkStmt->setInt(1, borrowId);
        sql::ResultSet *res = checkStmt->executeQuery();

        if (!res->next())
        {
            std::cout << "Borrowed record not found.\n";
            delete res;
            delete checkStmt;
            delete con;
            return;
        }

        bool returned = res->getBoolean("returned");
        if (returned)
        {
            std::cout << "Book already returned.\n";
            delete res;
            delete checkStmt;
            delete con;
            return;
        }

        int bookId = res->getInt("book_id");
        std::string borrowDate = res->getString("borrow_date");
        delete res;
        delete checkStmt;

        // Calculate penalty if returned late (due 14 days)
        time_t now = time(nullptr);

        // Convert borrowDate string "YYYY-MM-DD" to time_t
        struct tm tm_borrow = {};
        sscanf(borrowDate.c_str(), "%d-%d-%d", &tm_borrow.tm_year, &tm_borrow.tm_mon, &tm_borrow.tm_mday);
        tm_borrow.tm_year -= 1900; // tm_year since 1900
        tm_borrow.tm_mon -= 1;     // tm_mon 0-11
        time_t borrow_time = mktime(&tm_borrow);

        double seconds_diff = difftime(now, borrow_time);
        int days_diff = seconds_diff / (60 * 60 * 24);

        int penalty = 0;
        if (days_diff > 14)
        {
            penalty = (days_diff - 14) * 10; // Rs 10 per day late fee
        }

        // Update borrowed_books record to mark returned and add return_date = today
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
        std::string returnDate(buf);

        sql::PreparedStatement *updateStmt = con->prepareStatement(
            "UPDATE borrowed_books SET returned=1, return_date=? WHERE id=?");
        updateStmt->setString(1, returnDate);
        updateStmt->setInt(2, borrowId);
        updateStmt->execute();

        // Update book availability to true
        sql::PreparedStatement *bookUpdateStmt = con->prepareStatement("UPDATE books SET available=1 WHERE id=?");
        bookUpdateStmt->setInt(1, bookId);
        bookUpdateStmt->execute();

        std::cout << "✅ Book returned successfully.\n";
        if (penalty > 0)
        {
            std::cout << "⚠️ Penalty due to late return: Rs " << penalty << "\n";
        }

        delete updateStmt;
        delete bookUpdateStmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error returning book: " << e.what() << std::endl;
        delete con;
    }
}

void Borrow::viewBorrowedBooks()
{
    sql::Connection *con = connectDB();
    if (!con)
        return;

    try
    {
        sql::PreparedStatement *stmt;
        if (currentRole == "admin" || currentRole == "librarian")
        {
            // Show all borrowed books
            stmt = con->prepareStatement(
                "SELECT bb.id, u.username, b.name, bb.borrow_date, bb.return_date, bb.returned "
                "FROM borrowed_books bb "
                "JOIN users u ON bb.user_id = u.id "
                "JOIN books b ON bb.book_id = b.id "
                "ORDER BY bb.borrow_date DESC");
        }
        else
        {
            // Show only current user's borrowed books
            stmt = con->prepareStatement(
                "SELECT bb.id, b.name, bb.borrow_date, bb.return_date, bb.returned "
                "FROM borrowed_books bb "
                "JOIN users u ON bb.user_id = u.id "
                "JOIN books b ON bb.book_id = b.id "
                "WHERE u.username = ? "
                "ORDER BY bb.borrow_date DESC");
            stmt->setString(1, currentUser);
        }

        sql::ResultSet *res = stmt->executeQuery();

        std::cout << "\nBorrowed Books:\n";
        while (res->next())
        {
            int id = res->getInt("id");
            std::string bookName = res->getString("name");
            std::string borrowDate = res->getString("borrow_date");
            std::string returnDate = res->getString("return_date");
            bool returned = res->getBoolean("returned");

            std::cout << "\nRecord ID: " << id
                      << "\nBook: " << bookName
                      << "\nBorrowed on: " << borrowDate
                      << "\nReturned on: " << (returnDate.empty() ? "Not returned" : returnDate)
                      << "\nStatus: " << (returned ? "Returned" : "Not returned")
                      << "\n-------------------------";
        }

        delete res;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error fetching borrowed books: " << e.what() << std::endl;
        delete con;
    }
}
