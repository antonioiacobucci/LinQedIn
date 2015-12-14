#ifndef UTENTEBUSINESS_H
#define UTENTEBUSINESS_H
#include "utentebasic.h"

class UtenteBusiness: public UtenteBasic{
public:
    UtenteBusiness(Username u, Profilo p= Profilo(Info()), Rete* r=0);
private:
    UtenteBusiness* clone();
};

#endif // UTENTEBUSINESS_H
