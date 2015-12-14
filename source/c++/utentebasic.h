#ifndef UTENTEBASIC_H
#define UTENTEBASIC_H
#include "utente.h"

class UtenteBasic: public Utente{
public:
    UtenteBasic(Username u, Profilo p= Profilo(Info()), Rete* r=0);
private:
    UtenteBasic* clone();
};

#endif // UTENTEBASIC_H
