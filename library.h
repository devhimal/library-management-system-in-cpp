#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>

class Library
{
private:
    std::string currentUser;
    std::string currentRole;

public:
    Library(const std::string &user, const std::string &role);

    void addBook();
    void updateBook();
    void deleteBook();
    void displayBooks();
    void searchBook();
};

#endif // LIBRARY_H
