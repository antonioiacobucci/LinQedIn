#ifndef USERNAME_H
#define USERNAME_H
#include<string>
using namespace std;

class Username{
public:
    string username;
    Username();
    Username(string s);
    string getPw() const;
    bool matchPw(string pw)const ;          //controllo match password
    void setPw(string pw);
    bool operator==(Username un) const;
private:
    string password;
};

#endif // USERNAME_H
