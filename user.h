#ifndef USER_H
#define USER_H

#include <string>

class User
{
private:
    std::string username;
    std::string role;

public:
    User() = default;

    bool registerUser();
    bool loginUser();
    std::string getUsername() const;
    std::string getRole() const;
};

#endif // USER_H
