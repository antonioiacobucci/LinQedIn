#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <string>
#include <list>
#include <QObject>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <typeinfo>
#include "smartutente.h"
#include "utente.h"
#include "utentebasic.h"
#include "utentebusiness.h"
#include "utenteexecutive.h"
#include "eccezioni.h"
#include "rete.h"

typedef QDomElement El;

class Database : public QObject{
    Q_OBJECT
public:
    Database(string p= "", list<SmartUtente*> d=list<SmartUtente*>());
    ~Database();
    string path;
    list<SmartUtente*> db;
    bool empty() const;
    void clean();
    void setPath(string p);
    string getPath() const;
    bool checkLogin(string u, string p) const;                                  //controllo login
    bool controlPsw(Utente* u, string o, string n, string nc) const;            //controllo cambio password
    bool checkReg(string uR, string pR, string pcR) const;                      //controllo registrazione
    bool checkRegA(string uR, string pR) const;                                 //controllo registrazione da admin
    void load(string p);                                                        //carica da file
    void save();                                                                //salva su file
    Utente* find(Username u);                                                   //cerca utente per creazione client
    Utente* usersearch(Utente* req, Username un) const throw (Eccezioni::ErrSearch);        //ricerca
    Utente& operator[](SmartUtente it) const;
    void emit_err(int e);
signals:
    void err(int) const;
};

#endif // DATABASE_H
