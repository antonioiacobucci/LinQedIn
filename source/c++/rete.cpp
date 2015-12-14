#include <iostream>
#include "rete.h"

Rete::Rete(list<Username*> n): net(n){}

Rete::~Rete(){
    list<Username*>::iterator i = net.begin();
    for( ; i != net.end(); ++i) delete *i;
}

void Rete::add(Username* un){
    net.push_back(un);
}

void Rete::canc(Username* un){
    net.remove(un);
}

int Rete::nsize() const{
    return net.size();
}
