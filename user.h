#ifndef USER_H
#define USER_H

#include <string>

class User
{
private:
    std::string username;
    std::string role;

public:
    bool login();
    std::string getRole();
};

#endif
