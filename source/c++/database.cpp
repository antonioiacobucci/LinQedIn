#include <iostream>
#include <algorithm>
#include "database.h"

Database::Database(string p, list<SmartUtente*> d): path(p), db(d){}

Database::~Database(){
    list<SmartUtente*>::iterator i = db.begin();
    for( ; i != db.end(); ++i) delete *i;
}

bool Database::empty() const{
    return path == "";
}

void Database::clean(){
    if(db.size()){
        list<SmartUtente*>::iterator i=db.begin();
        for(;i!=db.end();++i){
            delete *i;
        }
        db.clear();
    }
}

void Database::setPath(string p){
    path= p;
}

string Database::getPath() const{
    return this->path;
}

bool Database::checkLogin(string u, string p) const{
    list<SmartUtente*>::const_iterator i = db.begin();
    for(; i != db.end(); ++i){
        if ((**i)->getUsername() == u){
           if((**i)->user.matchPw(p)) return true;
           else{
               emit err(1);
               return false;
           }
        }
    }
    emit err(0);
    return false;
}

bool Database::controlPsw(Utente* u, string o, string n, string nc) const{
    if(!(u->user.matchPw(o))){
        emit err(6);
        return false;
    }
    if(n.size() < 6){
         emit err(7);
         return false;
    }
    else if(n != nc){
         emit err(8);
         return false;
    }
    else return true;
}

bool Database::checkReg(string uR, string pR, string pcR) const{
    if(uR.size() < 4){
        emit err(5);
        return false;
    }
    list<SmartUtente*>::const_iterator i = db.begin();
    for(; i != db.end(); ++i){
        if ((**i)->getUsername() == uR){
            emit err(2);
            return false;
        }
    }
    if(pR.size() < 6){
         emit err(3);
         return false;
    }
    else if(pR != pcR){
         emit err(4);
         return false;
    }
    return true;
}

bool Database::checkRegA(string uR, string pR) const{
    if(uR.size() < 4){
        emit err(20);
        return false;
    }
    list<SmartUtente*>::const_iterator i = db.begin();
    for(; i != db.end(); ++i){
        if ((**i)->getUsername() == uR){
            emit err(18);
            return false;
        }
    }
    if(pR.size() < 6){
         emit err(19);
         return false;
    }
    return true;
}

Utente* Database::find(Username u) {
    Utente* user;
    list<SmartUtente*>::iterator i = db.begin();
    for(; i != db.end(); ++i){
        if ((**i)->getUsername() == u.username){
            user= (new SmartUtente(**i))->u;
        }
    }
    return user;
}

void Database::load(string p){
    cout<<"Caricamento in corso..."<<endl;
    try{
        if(p.empty()) return;
        if(!p.empty()) clean();
        QDomDocument doc("dbLinkedin");
        QFile file(QString::fromStdString(path));
        if (!file.open(QIODevice::ReadOnly | QIODevice::WriteOnly)){
            cout<<"Errore: permessi insufficienti \n";
            throw Eccezioni::BadPermission();
        }
        if (!doc.setContent(&file)){
            throw Eccezioni::BadPermission();
        }
        int n = 0;
        El users = doc.firstChildElement("users").firstChildElement("user");
        while(!users.isNull()){
            El pointer;
            Utente * s;
            if(users.attributeNode("type").value() == "Basic"){
                s= new UtenteBasic(Username(),Profilo(Info()), new Rete());
                s->setAccount((users.attributeNode("type").value()).toStdString());
            }
            else if(users.attributeNode("type").value() == "Business"){
                s= new UtenteBusiness(Username(),Profilo(Info()), new Rete());
                s->setAccount((users.attributeNode("type").value()).toStdString());
            }
            else if(users.attributeNode("type").value() == "Executive"){
                s= new UtenteExecutive(Username(),Profilo(Info()), new Rete());
                s->setAccount((users.attributeNode("type").value()).toStdString());
            }
            if(users.firstChildElement("name").isNull()==false)
                s->setName((users.firstChildElement("name").text()).toStdString());
            if(users.firstChildElement("surname").isNull()==false)
                s->setSurname((users.firstChildElement("surname").text()).toStdString());
            if(users.firstChildElement("username").isNull()==false)
                s->setUsername((users.firstChildElement("username").text()).toStdString());
            if(users.firstChildElement("password").isNull()==false)
                s->setPassword((users.firstChildElement("password").text()).toStdString());
            if(users.firstChildElement("genre").isNull()==false)
                s->setSesso((users.firstChildElement("genre").text()).toStdString());
            if(users.firstChildElement("city").isNull()==false)
                s->setCity((users.firstChildElement("city").text()).toStdString());
            if(users.firstChildElement("email").isNull()==false)
                s->setEmail((users.firstChildElement("email").text()).toStdString());
            if(users.firstChildElement("birth").isNull()==false)
                s->setBirth(QDate::fromString(users.firstChildElement("birth").text(),"yyyy'-'MM'-'dd"));
            list<string> q_list;
            pointer= users.firstChildElement("qualifications").firstChildElement("qualification");
            while(pointer.isNull()==false){
                q_list.push_back((pointer.text()).toStdString());
                pointer= pointer.nextSiblingElement("qualification");
            }
            list<Username*> r_list;
            pointer= users.firstChildElement("contacts").firstChildElement("contact");
            while(pointer.isNull()==false){
                r_list.push_back(new Username((pointer.text()).toStdString()));
                pointer= pointer.nextSiblingElement("contact");
            }
            s->setQualification(q_list);
            s->setRete(r_list);
            SmartUtente* su= new SmartUtente(s);
            this->db.push_back(su);
            users= users.nextSiblingElement("user");
            n++;
        }
        cout<<"Termine caricamento"<<endl;
        setPath(p);
        cout<<"Path settato"<<endl;
        file.close();

    }
    catch(Eccezioni::BadPermission){
        QMessageBox::information(0,"Caricamento non riuscito","E' possibile che non si disponga più dei permessi necessari per leggere il database");
        return;
    }
}

void Database::save(){
    cout<<"Salvataggio in corso..."<<endl;
    try{
        QFile * device = new QFile(QString::fromStdString(path));
        if (!device->open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Truncate)) {
            cout<<"Errore: permessi insufficienti \n";
            throw Eccezioni::BadPermission();
        }
        QXmlStreamWriter * out = new QXmlStreamWriter;
        out->setAutoFormatting(true);
        out->setDevice(device);
        out->writeStartDocument();
        out->writeStartElement("users");
        list<SmartUtente*>::iterator user = db.begin();
        for( ; user!=db.end(); ++user ){
            out->writeStartElement("user");
            Utente *us= (*user)->u;
            if(typeid(*us)== typeid(UtenteExecutive)) out->writeAttribute("type","Executive");
            else if(typeid(*us)== typeid(UtenteBusiness)) out->writeAttribute("type","Business");
            else if(typeid(*us)== typeid(UtenteBasic)) out->writeAttribute("type","Basic");
            out->writeTextElement("name",QString::fromStdString(us->getName()));
            out->writeTextElement("surname",QString::fromStdString(us->getSurname()));
            out->writeTextElement("username",QString::fromStdString(us->getUsername()));
            out->writeTextElement("password",QString::fromStdString(us->user.getPw()));
            out->writeTextElement("genre",QString::fromStdString(us->getSesso()));
            out->writeTextElement("city",QString::fromStdString(us->getCity()));
            out->writeTextElement("email",QString::fromStdString(us->getEmail()));
            out->writeTextElement("birth", us->getBirth().toString("yyyy'-'MM'-'dd"));
            out->writeStartElement("qualifications");
            if(us->prof.in.qsize()>0){
                list<string> q_list= us->getQualification();
                list<string>::iterator qf = q_list.begin();
                for( ; qf != q_list.end(); ++qf){
                    out->writeTextElement("qualification",QString::fromStdString(*qf));
                }
            }
            out->writeEndElement();     //fine qualifiche
            out->writeStartElement("contacts");
            if(us->rete->nsize() > 0){
                list<Username*> r_list= us->getRete();
                list<Username*>::iterator ct = r_list.begin();
                for( ; ct != r_list.end(); ++ct){
                    out->writeTextElement("contact",QString::fromStdString((*ct)->username));
                }
            }
            out->writeEndElement();     //fine contatti
            out->writeEndElement();     //fine utente
        }
        out->writeEndElement();         //fine utenti
        out->writeEndDocument();        //fine salvataggio
        device->close();                //chiusura file
        cout<<"Termine salvataggio"<<endl;
    }
    catch(Eccezioni::BadPermission){
        QMessageBox::information(0,"Salvataggio non riuscito","E' possibile che non si disponga più dei permessi necessari per scrivere il database");
        return;
    }
}

Utente* Database::usersearch(Utente* req, Username un) const throw(Eccezioni::ErrSearch) {
    bool trovato= false;
    Utente* contatto;
    SmartUtente* s;
    list<SmartUtente*>::const_iterator it= db.begin();
    for(; it!=db.end(); ++it){
        if(!trovato){
            if(((**it)->getUsername())==un.username){
                s=(*it);
                contatto=(*it)->u;
                trovato=true;
            }
        }
    }
    if(!trovato){
        throw Eccezioni::ErrSearch();
        return 0;
    }
    if(trovato){                                            //controllo il tipo dell'utente che richiede la ricerca
        if(typeid(*req)== typeid(UtenteBasic)) {
            SmartUtente::SearchFunctor search(1);
            contatto= search(s);
        }
        else if(typeid(*req)== typeid(UtenteBusiness)) {
            SmartUtente::SearchFunctor search(2);
            contatto= search(s);
        }
        else if(typeid(*req)== typeid(UtenteExecutive)) {
            SmartUtente::SearchFunctor search(3);
            contatto= search(s);
        }
        else contatto=0;
    }
    return contatto;
}

void Database::emit_err(int e){
    emit err(e);
}
