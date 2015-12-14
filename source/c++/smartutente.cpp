#include <iostream>
#include "smartutente.h"

SmartUtente::SmartUtente(){}

SmartUtente::SmartUtente(Utente* ut): u(ut){}

SmartUtente::SearchFunctor::SearchFunctor(int x): searchType(x) {}

Utente* SmartUtente::SearchFunctor::operator()(const SmartUtente* s) const{
    Utente* ut= (s->u);
    Utente* aux= ut->clone();                                                           //clono utente
    list<string> ql;
    list<Username*> rl;
    switch(searchType) {
        case(1):                                                                        //utente basic
            ql.push_back("Non puoi accedere a queste informazioni");
            rl.push_back(new Username("Non puoi accedere a queste informazioni"));
            aux->setQualification(ql);
            aux->setRete(rl);
            break;
        case(2):                                                                        //utente business
            rl.push_back(new Username("Non puoi accedere a queste informazioni"));
            aux->setRete(rl);
            break;
        case(3):                                                                        //utente executive
            break;
    }
    return aux;
}

bool SmartUtente::operator==(const SmartUtente& x) const{
    return *this==x;
}

Utente* SmartUtente::operator ->() const{
    return this->u;
}
