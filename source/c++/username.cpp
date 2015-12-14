#include <iostream>
#include "username.h"

Username::Username(){}

Username::Username(string s): username(s){}

string Username::getPw() const{
    return password;
}

bool Username::matchPw(string pw) const {
    return password == pw;
}

void Username::setPw(string pw){
    password = pw;
}

bool Username::operator==(Username un) const{
    return *this==un;
}
