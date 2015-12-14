#include <iostream>
#include "info.h"

Info::Info(string n, string c, string e, string a, string s, QDate d, list<string> t, string acc)
    :nome(n), cognome(c), email(e), city(a), sesso(s), data_nascita(d), titolo_studio(t), account(acc){}

int Info::qsize() const{
    return titolo_studio.size();
}

void Info::setAcc(string a){
    this->account= a;
}

string Info::getAcc() const{
    return this->account;
}
