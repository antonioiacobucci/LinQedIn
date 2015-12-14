#ifndef SMARTUTENTE_H
#define SMARTUTENTE_H
#include "utente.h"

class SmartUtente {
public:
    SmartUtente();
    SmartUtente(Utente* ut);
    Utente* u; // puntatore polimorfo
    bool operator==(const SmartUtente& x) const;
    Utente* operator->() const;
    class SearchFunctor {                                   //funtore
    public:
        int searchType;
        SearchFunctor(int x=1);
        Utente* operator() (const SmartUtente* s) const;
    };
};

#endif // SMARTUTENTE_H
