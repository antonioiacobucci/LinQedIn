#include <iostream>
#include "utentebusiness.h"

UtenteBusiness::UtenteBusiness(Username u, Profilo p, Rete*r): UtenteBasic(u,p,r){}

UtenteBusiness* UtenteBusiness::clone(){
    Username u_clone= Username(this->getUsername());
    Info i_clone= Info(
                this->getName(),
                this->getSurname(),
                this->getEmail(),
                this->getCity(),
                this->getSesso(),
                this->getBirth(),
                this->getQualification(),
                this->getAccount()
                );
    Profilo p_clone= Profilo(i_clone);
    list<Username*> l_clone;
    list<Username*>::iterator i = this->rete->net.begin();
    for( ; i != this->rete->net.end(); ++i){
        l_clone.push_back(*i);
    }
    Rete* r_clone= new Rete(l_clone);
    return new UtenteBusiness(u_clone,p_clone,r_clone);
}

