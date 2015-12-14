#ifndef PROFILO_H
#define PROFILO_H
#include "info.h"

class Profilo {
public:
    Info in;
    void modificaProfilo(string n="",string s="", string e="", string c="", string g="", QDate d=QDate(), string q="");
    Profilo(Info i);
};

#endif // PROFILO_H
