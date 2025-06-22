#include <iostream>
#include <limits>
#include "user.h"
#include "library.h"
#include "borrow.h"

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
    std::cout << "=== 📚 Library Management System ===\n";

    User user;
    int option;

    while (true)
    {
        std::cout << "\n1. Register\n2. Login\n0. Exit\nChoose option: ";
        std::cin >> option;
        if (std::cin.fail())
        {
            clearInput();
            std::cout << "Invalid input\n";
            continue;
        }
        if (option == 1)
        {
            if (user.registerUser())
            {
                std::cout << "You can now login.\n";
            }
        }
        else if (option == 2)
        {
            if (user.loginUser())
            {
                break;
            }
        }
        else if (option == 0)
        {
            std::cout << "Goodbye!\n";
            return 0;
        }
        else
        {
            std::cout << "Invalid option\n";
        }
    }

    Library library(user.getUsername(), user.getRole());
    Borrow borrow(user.getUsername(), user.getRole());

    int choice;
    do
    {
        std::cout << "\nWelcome, " << user.getUsername() << " (" << user.getRole() << ")\n";
        std::cout << "====== Menu ======\n";
        std::cout << "1. Display Books\n2. Search Books\n";
        if (user.getRole() == "admin" || user.getRole() == "librarian")
        {
            std::cout << "3. Add Book\n4. Update Book\n5. Delete Book\n";
            std::cout << "6. Issue (Borrow) Book\n7. Return Book\n8. View Borrowed Books\n";
            std::cout << "9. Manage Users (Admin Only)\n";
        }
        else
        {
            std::cout << "6. View My Borrowed Books\n";
        }
        std::cout << "0. Exit\nEnter choice: ";
        std::cin >> choice;

        if (std::cin.fail())
        {
            clearInput();
            std::cout << "Invalid input.\n";
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
            if (user.getRole() == "admin" || user.getRole() == "librarian")
                library.addBook();
            else
                std::cout << "Access denied.\n";
            break;
        case 4:
            if (user.getRole() == "admin" || user.getRole() == "librarian")
                library.updateBook();
            else
                std::cout << "Access denied.\n";
            break;
        case 5:
            if (user.getRole() == "admin" || user.getRole() == "librarian")
                library.deleteBook();
            else
                std::cout << "Access denied.\n";
            break;
        case 6:
            if (user.getRole() == "admin" || user.getRole() == "librarian")
                borrow.borrowBook();
            else
                borrow.viewBorrowedBooks();
            break;
        case 7:
            if (user.getRole() == "admin" || user.getRole() == "librarian")
                borrow.returnBook();
            else
                std::cout << "Access denied.\n";
            break;
        case 8:
            if (user.getRole() == "admin" || user.getRole() == "librarian")
                borrow.viewBorrowedBooks();
            else
                std::cout << "Access denied.\n";
            break;
        case 9:
            if (user.getRole() == "admin")
            {
                std::cout << "User management not implemented yet.\n";
            }
            else
            {
                std::cout << "Access denied.\n";
            }
            break;
        case 0:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid option.\n";
        }

    } while (choice != 0);

    return 0;
}
