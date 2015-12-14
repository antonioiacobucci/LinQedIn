#ifndef HEADER_H
#define HEADER_H
#include "profilo.h"
#include "username.h"
#include "rete.h"
#include <list>
#include <sstream>
#include <eccezioni.h>

class Rete;
class Database;

class Utente{
public:
    Username user;
    Profilo prof;
    Rete* rete;
    Utente(Username u, Profilo p= Profilo(Info()), Rete* r= 0);
    virtual ~Utente();
    virtual void setName(string n);
    virtual void setSurname(string s);
    virtual void setUsername(string u);
    virtual void setPassword(string p);
    virtual void setSesso(string g);
    virtual void setCity(string c);
    virtual void setEmail(string e);
    virtual void setBirth(QDate b);
    virtual void addQualification(string q);
    virtual void setQualification(list<string> q_list);
    virtual void addRete(string r);
    virtual void setRete(list<Username*> r_list);
    virtual void setAccount(string a);
    virtual string getName() const;
    virtual string getSurname() const;
    virtual string getUsername() const;
    virtual string getSesso() const;
    virtual string getCity() const;
    virtual string getEmail() const;
    virtual QDate getBirth() const;
    virtual list<string> getQualification() const;
    virtual list<Username*> getRete() const;
    virtual string getAccount()const;
    virtual Utente * clone() = 0;           //funzionalità di copia
};

#endif // HEADER_H
