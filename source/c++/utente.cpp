#include <iostream>
#include "utente.h"

Utente::Utente(Username u, Profilo p, Rete* r):user(u), prof(p), rete(r){}

Utente::~Utente(){}

void Utente::setName(string n){
    this->prof.in.nome= n;
}

void Utente::setSurname(string s){
    this->prof.in.cognome= s;
}

void Utente::setUsername(string u){
    this->user.username= u;
}

void Utente::setPassword(string p){
    this->user.setPw(p);
}

void Utente::setSesso(string g){
    this->prof.in.sesso= g;
}

void Utente::setCity(string c){
    this->prof.in.city= c;
}

void Utente::setEmail(string e){
    this->prof.in.email= e;
}

void Utente::addQualification(string q){
    this->prof.in.titolo_studio.push_back(q);
}

void Utente::setQualification(list<string> q_list){
    this->prof.in.titolo_studio = q_list;
}

void Utente::setBirth(QDate b){
    this->prof.in.data_nascita= b;
}

void Utente::addRete(string r){
    Username* un= new Username(r);
    this->rete->add(un);
}

void Utente::setRete(list<Username*> r_list){
    this->rete->net = r_list;
}

void Utente::setAccount(string a){
    this->prof.in.setAcc(a);
}

string Utente::getName() const{
    return this->prof.in.nome;
}

string Utente::getSurname() const{
    return this->prof.in.cognome;
}

string Utente::getUsername() const{
    return this->user.username;
}

string Utente::getSesso() const{
    return this->prof.in.sesso;
}

string Utente::getCity() const{
    return this->prof.in.city;
}

string Utente::getEmail() const{
    return this->prof.in.email;
}

QDate Utente::getBirth() const{
    return this->prof.in.data_nascita;
}

list<string> Utente::getQualification() const{
     return this->prof.in.titolo_studio;
}

list<Username *> Utente::getRete() const{
    return this->rete->net;
}

string Utente::getAccount() const{
    return this->prof.in.getAcc();
}

