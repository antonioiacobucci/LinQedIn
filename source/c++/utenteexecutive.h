#ifndef UTENTEEXECUTIVE_H
#define UTENTEEXECUTIVE_H
#include "utentebusiness.h"

class UtenteExecutive: public UtenteBusiness{
public:
    UtenteExecutive(Username u, Profilo p= Profilo(Info()), Rete* r=0);
private:
    UtenteExecutive* clone();
};

#endif // UTENTEEXECUTIVE_H
