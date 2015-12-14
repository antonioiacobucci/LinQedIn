#ifndef RETE_H
#define RETE_H
#include <username.h>
#include <list>

class Rete {
public:
    Rete(list<Username*> n= list<Username*>());
    ~Rete();
    list<Username*> net;
    int nsize() const;
    void add(Username* un);     //aggiungi contatto
    void canc(Username *un);    //rimuovi contatto
};

#endif // RETE_H
