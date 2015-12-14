#include <iostream>
#include "client.h"
#include "eccezioni.h"

Client::Client(Database* db, Utente* ut): data(db), u(ut){}

void Client::aggiornaProfilo(string n,string s, string e, string c, string g, QDate d, string q){
    (u->prof).modificaProfilo(n,s,e,c,g,d,q);
}

void Client::insertRete(Username* un){
    u->rete->add(un);
}

void Client::removeRete(Username* un){
    u->rete->canc(un);
}

void Client::removeQualifica(string s){
    u->prof.in.titolo_studio.remove(s);
}

Utente* Client::ricerca(Username un) const throw(Eccezioni::ErrSearch){
    Utente* contatto= data->usersearch(this->u,un);
    return contatto;
}
