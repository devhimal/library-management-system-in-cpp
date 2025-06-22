#ifndef BORROW_H
#define BORROW_H

#include <string>

class Borrow
{
private:
    std::string currentUser;
    std::string currentRole;

public:
    Borrow(const std::string &user, const std::string &role);

    void borrowBook();
    void returnBook();
    void viewBorrowedBooks();
    void manageUsers(); // Optional: Admin/Librarian user management stub
};

#endif // BORROW_H
