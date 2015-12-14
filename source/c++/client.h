#ifndef CLIENT_H
#define CLIENT_H
#include "rete.h"
#include "eccezioni.h"
#include "database.h"

class Client {   //lato client
public:
Database* data;
Utente* u;
Client(Database* db,Utente* ut);
void aggiornaProfilo(string n=0,string s=0, string e=0, string c=0, string g=0, QDate d=QDate(), string q=0);
void insertRete(Username* un);                                      //inserisci contatto
void removeRete(Username* un);                                      //rimuovi contatto
void removeQualifica(string s);                                     //rimuovi qualifica
Utente* ricerca(Username un) const throw (Eccezioni::ErrSearch);    //ricerca contatto
};

#endif // CLIENT_H
