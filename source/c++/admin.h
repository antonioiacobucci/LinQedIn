#ifndef ADMIN_H
#define ADMIN_H
#include "database.h"

class Admin {  //lato amministratore
public:
Database* data;
Admin(Database* db);
bool changeSubscriptionType(Username un, string type);      //cambia tipo account
bool insert(string u, string p, string a);                  //inserisci utente
bool remove(string s);                                      //rimuovi utente da database e contatti di altri utenti
Utente* cerca(Username* us) const;                          //cerca utente
};

#endif // ADMIN_H
