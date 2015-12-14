#include <iostream>
#include "admin.h"

Admin::Admin(Database *db):data(db){}

bool Admin::changeSubscriptionType(Username un, string type) {
    if((type!= "Basic") && (type !="Business") && (type!="Executive")){             //tipo errato
        emit data->emit_err(10);
        return false;
    }
    bool trovato=false;
    Utente* ut, *newUser;
    list<SmartUtente*>::iterator it= data->db.begin();
    for(; it!=data->db.end(); ++it){
        if(!trovato){
            if(((**it)->getUsername())==un.username){       //trovato utente
                trovato=true;
                ut= (*it)->u;
                string pw= ut->user.getPw();
                if (ut->getAccount() != type){              //esegue il cambiamento solo se il tipo è diverso da quello attuale
                    if(type== "Basic"){
                        newUser= new UtenteBasic(ut->getUsername(), ut->prof, ut->rete);
                        newUser->setPassword(pw);
                    }
                    else if(type=="Business"){
                        newUser= new UtenteBusiness(ut->getUsername(), ut->prof, ut->rete);
                        newUser->setPassword(pw);
                    }
                    else if(type=="Executive"){
                        newUser= new UtenteExecutive(ut->getUsername(), ut->prof, ut->rete);
                        newUser->setPassword(pw);
                    }
                    data->db.remove(*it);                           //rimuove vecchio utente
                    data->db.push_back(new SmartUtente(newUser));   //aggiungi utente con nuovo tipo
                    return true;
                }
                else{
                    emit data->emit_err(22);
                    return false;
                }
            }
        }
    }
    emit data->emit_err(9);
    return false;
}

bool Admin::insert(string u, string p, string a){
    if((a!= "Basic") && (a !="Business") && (a!="Executive")){
        emit data->emit_err(17);
        return false;
    }
    Username us= Username(u);
    us.setPw(p);
    Utente* ut;
    if(a=="Basic") ut= new UtenteBasic(us);
    else if(a=="Business") ut= new UtenteBusiness(us);
    else if(a=="Executive") ut= new UtenteExecutive(us);
    list<Username*> r;
    ut->rete = new Rete(r);
    SmartUtente *s= new SmartUtente(ut);
    data->db.push_back(s);
    return true;
}

bool Admin::remove(string s){
    bool rimosso= false;
    list<SmartUtente*>::iterator it= data->db.begin();
    for(; it!=data->db.end(); ++it){
        list<Username*> n_list= (*it)->u->getRete();
        list<Username*>::iterator cont = n_list.begin();
        for( ; cont!=n_list.end(); ++cont ){
            if((**cont).username == s) (*it)->u->rete->net.remove(*cont);
        }
        if(((**it)->getUsername())== s){
            rimosso=true;
            data->db.remove(*it);
        }
    }
    return rimosso;
}

Utente* Admin::cerca(Username *us) const{
    bool trovato= false;
    Utente* contatto;
    list<SmartUtente*>::const_iterator it= data->db.begin();
    for(; it!=data->db.end(); ++it){
        if(!trovato){
            if(((**it)->getUsername())==us->username){
                contatto=(*it)->u;
                trovato=true;
            }
        }
    }
    if(!trovato) return 0;
    else return contatto;
}
