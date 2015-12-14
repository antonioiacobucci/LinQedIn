#include <iostream>
#include "utenteexecutive.h"

UtenteExecutive::UtenteExecutive(Username u, Profilo p, Rete* r): UtenteBusiness(u,p,r){}

UtenteExecutive* UtenteExecutive::clone(){
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
    return new UtenteExecutive(u_clone,p_clone,r_clone);
}
