#ifndef INFO_H
#define INFO_H
#include <list>
#include <QDate>

using namespace std;

class Info{
public:
    Info(string n="", string c="", string e="", string a="", string s="", QDate d= QDate(), list<string> t= list<string>(), string acc="");
    string nome;
    string cognome;
    string email;
    string city;
    string sesso;
    QDate data_nascita;
    list<string> titolo_studio;
    int qsize() const;
    void setAcc(string a);
    string getAcc() const;
private:
    string account;
};

#endif // INFO_H
