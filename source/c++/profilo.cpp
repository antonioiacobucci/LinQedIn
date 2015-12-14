#include <iostream>
#include "profilo.h"

Profilo::Profilo(Info i):in(i){}

void Profilo::modificaProfilo(string n, string s, string e, string c, string g, QDate d, string q){
    this->in.nome=n;
    this->in.cognome=s;
    this->in.email=e;
    this->in.city=c;
    this->in.sesso=g;
    this->in.data_nascita=d;
    if(q != "") this->in.titolo_studio.push_back(q);
}
