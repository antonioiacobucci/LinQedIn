#include<iostream>
#include "Widget.h"

Widget::Widget(){
    db=new Database("./data.xml");
    setWindowTitle("LinQedIn");
    resize(750, 450);
    statusBar()->showMessage(tr("Benvenuto su LinQedIn"));
    loggedA=false;
    loggedC=false;
    QWidget *widget = new QWidget();
    mainLayout = new QVBoxLayout;
    createView();
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
    connect(db,SIGNAL(err(int)),this, SLOT(error(int)));
}

void Widget::clearLayout(QLayout* l){
    while((layout->count() > 0 )) {
        QLayoutItem *item= l->takeAt(0);
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    delete gBox;
}

void Widget::login() {
    if((usedit->text().toStdString()== "admin")&& (pwedit->text().toStdString()=="password")){  //login admin
        amministratore= new Admin(db);
        loggedA= true;
        statusBar()->showMessage(tr("Autenticazione eseguita"));
        fileMenu->removeAction(logAct);
        fileMenu->removeAction(regAct);
        fileMenu->removeAction(exitAct);
        logAct= fileMenu->addAction(tr("&Logout"));
        logAct->setStatusTip(tr("Disconnetti account"));
        fileMenu->addSeparator();
        exitAct = fileMenu->addAction(tr("&Exit"));
        exitAct->setShortcuts(QKeySequence::Quit);
        exitAct->setStatusTip(tr("Esci dall'applicazione"));
        connect(logAct, SIGNAL(triggered()), this, SLOT(redirect()));
        connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
        createAdminView();
    }
    else if (db->checkLogin(usedit->text().toStdString(), pwedit->text().toStdString())){       //login client
        Utente* ut= db->find(Username(usedit->text().toStdString()));
        utente= new Client(db, ut);
        loggedC= true;
        statusBar()->showMessage(tr("Autenticazione eseguita"));
        fileMenu->removeAction(logAct);
        fileMenu->removeAction(regAct);
        fileMenu->removeAction(exitAct);
        logAct= fileMenu->addAction(tr("&Logout"));
        logAct->setStatusTip(tr("Disconnetti account"));
        fileMenu->addSeparator();
        exitAct = fileMenu->addAction(tr("&Exit"));
        exitAct->setShortcuts(QKeySequence::Quit);
        exitAct->setStatusTip(tr("Esci dall'applicazione"));
        searchMenu->menuAction()->setVisible(true);
        connect(logAct, SIGNAL(triggered()), this, SLOT(redirect()));
        connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
        createAuthView();
    }
}

void Widget::logout() {
    if(loggedC == true){
        delete utente;
        loggedC= false;
        statusBar()->showMessage(tr("Disconnessione eseguita"));
    }
    else if(loggedA == true){
        delete amministratore;
        loggedA= false;
        statusBar()->showMessage(tr("Disconnessione eseguita"));
    } 
    fileMenu->removeAction(logAct);
    fileMenu->removeAction(regAct);
    fileMenu->removeAction(exitAct);
    logAct= fileMenu->addAction(tr("&Login"));
    logAct->setStatusTip(tr("Autentica il tuo account"));
    regAct= fileMenu->addAction(tr("&Register"));
    regAct->setStatusTip(tr("Crea nuovo account"));
    fileMenu->addSeparator();
    exitAct = fileMenu->addAction(tr("&Exit"));
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Esci dall'applicazione"));
    searchMenu->menuAction()->setVisible(false);
    db->load(db->getPath());
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(logAct, SIGNAL(triggered()), this, SLOT(redirect()));
    connect(regAct, SIGNAL(triggered()), this, SLOT(redirect()));
}

void Widget::regAcc(){
    if (db->checkReg(useditR->text().toStdString(), pweditR->text().toStdString(),pwceditR->text().toStdString())){
        Username us= Username(useditR->text().toStdString());
        us.setPw(pweditR->text().toStdString());
        UtenteBasic* ub= new UtenteBasic(us);
        list<Username*> r;
        ub->rete = new Rete(r);
        SmartUtente *s= new SmartUtente(ub);
        db->db.push_back(s);
        db->save();
        statusBar()->showMessage(tr("Registrazione eseguita"));
        logAct->trigger();
    }
}

void Widget::changeScreen(){
    if (windowState() == Qt::WindowFullScreen) showNormal();
    else showFullScreen();
}

void Widget::changeType(){
    if(amministratore->changeSubscriptionType(useredit->text().toStdString(), subedit->text().toStdString())){
        db->save();
        db->load(db->getPath());
        createAdminView();
        emit db->emit_err(14);
    }
}

void Widget::editInfo(){
    string n=nmedit->text().toStdString();
    string s=smedit->text().toStdString();
    string e=emedit->text().toStdString();
    string c=cyedit->text().toStdString();
    string g;
    if(sxmod->currentText().toStdString()== "-") g= "";
    else g= sxmod->currentText().toStdString();
    QDate d= QDate::fromString(bhedit->text(),"yyyy'-'MM'-'dd");
    if (d.isValid()==false) d= QDate::fromString("");
    string q =addQedit->text().toStdString();
    list<string> q_list= utente->u->getQualification();
    list<string>::iterator qiter = q_list.begin();
    for( ; qiter != q_list.end(); ++qiter){
        if((*qiter)== q)
        string q ="";
    }
    utente->aggiornaProfilo(n,s,e,c,g,d,q);
    db->save();
    statusBar()->showMessage(tr("Profilo modificato"));
}

void Widget::changePsw(){
    if (db->controlPsw(utente->u, oldedit->text().toStdString(), newedit->text().toStdString(), newcedit->text().toStdString())){
        utente->u->setPassword(newedit->text().toStdString());
        db->save();
        statusBar()->showMessage(tr("Password modificata"));
        createAuthView();
    }
}

void Widget::removeQ(){
    string q_item= qlw->currentItem()->text().toStdString();
    bool trovato=false;
    list<string> q_list= utente->u->getQualification();
    list<string>::iterator q = q_list.begin();
    for(; q != q_list.end(); ++q){
        if((*q)== q_item){
            trovato=true;
            utente->removeQualifica(*q);
            db->save();
            statusBar()->showMessage(tr("Qualifica rimossa"));
        }
    }
    clearLayout(layout);
    createModForm();
}

void Widget::searchUser(){
    bool trovato= false, contatto= false;
    list<SmartUtente*>::const_iterator user = db->db.begin();
    for(; user != db->db.end(); ++user){
        if(!trovato){
            if ((**user)->getUsername() == searchedit->text().toStdString()){
                trovato= true;
                list<Username*> n_list=utente->u->getRete();
                list<Username*>::iterator cont = n_list.begin();
                for( ; cont!=n_list.end(); ++cont ){
                    if(!contatto){
                        if((**cont).username == searchedit->text().toStdString()){
                            contatto= true;
                            viewCont(**cont);
                        }
                    }
                }
                if(contatto==false){
                    if(((*user)->u)== utente->u) createAuthView();
                    else viewUser((*user)->u);
                }
            }
        }
    }
    if(trovato==false) emit db->emit_err(16);
}

void Widget::viewUser(Utente *u){
    clearLayout(layout);
    gBox = new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Profilo utente"));
    us=new QLabel(tr("&Username"));
    usedit = new QLineEdit;
    usedit->setText(QString::fromStdString(u->getUsername()));
    usedit->setReadOnly(true);
    us->setBuddy(usedit);
    nm=new QLabel(tr("&Nome"));
    nmedit = new QLineEdit;
    nmedit->setText(QString::fromStdString(u->getName()));
    nmedit->setReadOnly(true);
    nm->setBuddy(nmedit);
    sm=new QLabel(tr("&Cognome"));
    smedit = new QLineEdit;
    smedit->setText(QString::fromStdString(u->getSurname()));
    smedit->setReadOnly(true);
    sm->setBuddy(smedit);
    backP= new QPushButton(tr("&Indietro"));
    backP->setFixedSize(QSize(70,25));
    text= new QLabel;
    text->setText("Non sono disponibili ulteriori informazioni poiche' l'utente non e' tra i propri contatti");
    text->setAlignment(Qt::AlignHCenter);
    QFont textFont("Arial", 10, QFont::Bold);
    text->setFont(textFont);
    layout->addWidget(text);
    layout->addRow(us, usedit);
    layout->addRow(nm, nmedit);
    layout->addRow(sm, smedit);
    layout->addWidget(backP);
    gBox->setLayout(layout);
    mainLayout->addWidget(gBox);
    connect(backP, SIGNAL(clicked()), this, SLOT(redirect()));
}

void Widget::viewCont(Username c){
    clearLayout(layout);
    gBox = new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Profilo contatto"));
    Utente* contatto= utente->ricerca(c);
    us=new QLabel(tr("&Username"));
    usedit = new QLineEdit;
    usedit->setText(QString::fromStdString(contatto->getUsername()));
    usedit->setReadOnly(true);
    us->setBuddy(usedit);
    nm=new QLabel(tr("&Nome"));
    nmedit = new QLineEdit;
    nmedit->setText(QString::fromStdString(contatto->getName()));
    nmedit->setReadOnly(true);
    nm->setBuddy(nmedit);
    sm=new QLabel(tr("&Cognome"));
    smedit = new QLineEdit;
    smedit->setText(QString::fromStdString(contatto->getSurname()));
    smedit->setReadOnly(true);
    sm->setBuddy(smedit);
    em=new QLabel(tr("&Email"));
    emedit = new QLineEdit;
    emedit->setText(QString::fromStdString(contatto->getEmail()));
    emedit->setReadOnly(true);
    em->setBuddy(emedit);
    cy=new QLabel(tr("&Citta"));
    cyedit = new QLineEdit;
    cyedit->setText(QString::fromStdString(contatto->getCity()));
    cyedit->setReadOnly(true);
    cy->setBuddy(cyedit);
    sx=new QLabel(tr("&Sesso"));
    sxedit = new QLineEdit;
    sxedit->setText(QString::fromStdString(contatto->getSesso()));
    sxedit->setReadOnly(true);
    sx->setBuddy(sxedit);
    bh=new QLabel(tr("&Data di nascita"));
    bhedit = new QLineEdit;
    bhedit->setText(contatto->getBirth().toString("yyyy'-'MM'-'dd"));
    bhedit->setReadOnly(true);
    bh->setBuddy(bhedit);
    qu=new QLabel(tr("&Qualifiche"));
    int qsize= contatto->getQualification().size();
    qlw= new QListWidget;
    list<string> q_list=contatto->getQualification();
    list<string>::iterator qiter = q_list.begin();
    for( ; qiter != q_list.end(); ++qiter){
        qlw->addItem(QString::fromStdString(*qiter));
    }
    cu=new QLabel(tr("&Contatti"));
    int nsize= contatto->getRete().size();
    nlw= new QListWidget;
    list<Username*> n_list= contatto->getRete();
    list<Username*>::iterator niter = n_list.begin();
    for( ; niter != n_list.end(); ++niter){
        nlw->addItem(QString::fromStdString((**niter).username));
    }
    backP= new QPushButton(tr("&Indietro"));
    backP->setFixedSize(QSize(70,25));
    layout->addRow(us, usedit);
    layout->addRow(nm, nmedit);
    layout->addRow(sm, smedit);
    layout->addRow(em, emedit);
    layout->addRow(cy, cyedit);
    layout->addRow(sx, sxedit);
    layout->addRow(bh, bhedit);
    if(qsize==0){
        quedit= new QLineEdit;
        quedit->setReadOnly(true);
        qu->setBuddy(quedit);
        layout->addRow(qu, quedit);
    }
    else if(qsize>0){
        qu->setBuddy(qlw);
        layout->addRow(qu,qlw);
    }
    if(nsize == 0){
        cuedit= new QLineEdit;
        cuedit->setReadOnly(true);
        cu->setBuddy(cuedit);
        layout->addRow(cu, cuedit);
    }
    else if(nsize>0){
        cu->setBuddy(nlw);
        layout->addRow(cu, nlw);
    }
    layout->addWidget(backP);
    gBox->setLayout(layout);
    mainLayout->addWidget(gBox);
    connect(backP, SIGNAL(clicked()), this, SLOT(redirect()));
}

void Widget::adminSearch(){
    Username* un= new Username(searchedit->text().toStdString());
    Utente* ut= amministratore->cerca(un);
    if(!ut) statusBar()->showMessage(tr("Utente non trovato"));
    if(ut){
        clearLayout(layout);
        gBox = new QGroupBox;
        layout = new QFormLayout;
        gBox->setTitle(tr("Profilo utente"));
        us=new QLabel(tr("&Username"));
        usedit = new QLineEdit;
        usedit->setText(QString::fromStdString(ut->getUsername()));
        usedit->setReadOnly(true);
        us->setBuddy(usedit);
        nm=new QLabel(tr("&Nome"));
        nmedit = new QLineEdit;
        nmedit->setText(QString::fromStdString(ut->getName()));
        nmedit->setReadOnly(true);
        nm->setBuddy(nmedit);
        sm=new QLabel(tr("&Cognome"));
        smedit = new QLineEdit;
        smedit->setText(QString::fromStdString(ut->getSurname()));
        smedit->setReadOnly(true);
        sm->setBuddy(smedit);
        em=new QLabel(tr("&Email"));
        emedit = new QLineEdit;
        emedit->setText(QString::fromStdString(ut->getEmail()));
        emedit->setReadOnly(true);
        em->setBuddy(emedit);
        cy=new QLabel(tr("&Citta"));
        cyedit = new QLineEdit;
        cyedit->setText(QString::fromStdString(ut->getCity()));
        cyedit->setReadOnly(true);
        cy->setBuddy(cyedit);
        sx=new QLabel(tr("&Sesso"));
        sxedit = new QLineEdit;
        sxedit->setText(QString::fromStdString(ut->getSesso()));
        sxedit->setReadOnly(true);
        sx->setBuddy(sxedit);
        bh=new QLabel(tr("&Data di nascita"));
        bhedit = new QLineEdit;
        bhedit->setText(ut->getBirth().toString("yyyy'-'MM'-'dd"));
        bhedit->setReadOnly(true);
        bh->setBuddy(bhedit);
        ac=new QLabel(tr("&Account"));
        acedit = new QLineEdit;
        acedit->setText(QString::fromStdString(ut->getAccount()));
        acedit->setReadOnly(true);
        ac->setBuddy(acedit);
        qu=new QLabel(tr("&Qualifiche"));
        int qsize= ut->getQualification().size();
        qlw= new QListWidget;
        list<string> q_list=ut->getQualification();
        list<string>::iterator qiter = q_list.begin();
        for( ; qiter != q_list.end(); ++qiter){
            qlw->addItem(QString::fromStdString(*qiter));
        }
        qlw->setMaximumHeight(qlw->sizeHintForColumn(0));
        cu=new QLabel(tr("&Contatti"));
        list<Username*> n_list= ut->getRete();
        int nsize= n_list.size();
        nlw= new QListWidget;
        list<Username*>::iterator niter = n_list.begin();
        for( ; niter != n_list.end(); ++niter){
            nlw->addItem(QString::fromStdString((**niter).username));
        }
        nlw->setMaximumHeight(nlw->sizeHintForColumn(0));
        backA= new QPushButton(tr("&Indietro"));
        backA->setFixedSize(QSize(70,25));
        layout->addRow(us, usedit);
        layout->addRow(nm, nmedit);
        layout->addRow(sm, smedit);
        layout->addRow(em, emedit);
        layout->addRow(cy, cyedit);
        layout->addRow(sx, sxedit);
        layout->addRow(bh, bhedit);
        layout->addRow(ac, acedit);
        if(qsize==0){
            quedit= new QLineEdit;
            quedit->setReadOnly(true);
            qu->setBuddy(quedit);
            layout->addRow(qu, quedit);
        }
        else if(qsize>0){
            qu->setBuddy(qlw);
            layout->addRow(qu,qlw);
        }
        if(nsize == 0){
            cuedit= new QLineEdit;
            cuedit->setReadOnly(true);
            cu->setBuddy(cuedit);
            layout->addRow(cu, cuedit);
        }
        else if(nsize>0){
            cu->setBuddy(nlw);
            layout->addRow(cu, nlw);
        }
        layout->addWidget(backA);
        layout->setContentsMargins(100,0,100,0);
        gBox->setLayout(layout);
        mainLayout->addWidget(gBox);
        connect(backA, SIGNAL(clicked()), this, SLOT(redirect()));
    }
}

void Widget::redirect(){
    if(sender()==modP){
        clearLayout(layout);
        createModForm();
    }
    else if(sender()==pswP){
        clearLayout(layout);
        createPswForm();
    }
    else if(sender()==backP){
        createAuthView();
    }
    else if(sender()==contP){
        clearLayout(layout);
        createContView();
    }
    else if(sender()==logAct){
        logout();
        clearLayout(layout);
        createLogForm();
    }
    else if((sender()==regAct) || (sender()==clickreg)){
        clearLayout(layout);
        createRegForm();
    }
    else if(sender()== searchAct){
        clearLayout(layout);
        createSearchForm();
    }
    else if(sender()==confirm){
        editInfo();
        createAuthView();
    }
    else if(sender()==addQ){
        addQedit->setVisible(true);
    }
    else if(sender()==addC){
        addCedit->setVisible(true);
        confCont->setVisible(true);
    }
    else if(sender()==remC){
        remCedit->setVisible(true);
        confCont->setVisible(true);
    }
    else if(sender()==confCont){
        editNet();
        clearLayout(layout);
        createContView();
    }
    else if(sender()== butt_change){
        user->setVisible(true);
        useredit->setVisible(true);
        sub->setVisible(true);
        subedit->setVisible(true);
        confirm->setVisible(true);
    }
    else if(sender()==qlw){
        remQ->setVisible(true);
    }
    else if(sender()==nlw){
        profC->setVisible(true);
    }
    else if(sender()==profC){
        string n_item= nlw->currentItem()->text().toStdString();
        list<Username*> n_list=utente->u->getRete();
        list<Username*>::iterator cont = n_list.begin();
        for( ; cont!=n_list.end(); ++cont ){
            if((**cont).username == n_item){
                viewCont(**cont);
            }
        }
    }
    else if(sender()==remU){
        if(amministratore->remove(remUedit->text().toStdString())){
            db->save();
            createAdminView();
            statusBar()->showMessage(tr("Utente rimosso"));
        }
        else statusBar()->showMessage(tr("Utente non presente nel database"));
    }
    else if(sender()==addU){
        us->setVisible(true);
        usedit->setVisible(true);
        pw->setVisible(true);
        pwedit->setVisible(true);
        ac->setVisible(true);
        acedit->setVisible(true);
        insert->setVisible(true);
    }
    else if(sender()==insert){
        if (db->checkRegA(usedit->text().toStdString(), pwedit->text().toStdString())){
            if(amministratore->insert(usedit->text().toStdString(), pwedit->text().toStdString(),acedit->text().toStdString())){
                db->save();
                createAdminView();
                db->emit_err(21);
            }
        }
    }
    else if(sender()==backA){
        createAdminView();;
    }
}


void Widget::editNet(){
    if(addCedit->text().toStdString() != "") insertC(addCedit->text().toStdString());
    else if(remCedit->text().toStdString() != "") removeC(remCedit->text().toStdString());
}

void Widget::goAbout(){
    about= new QDialog(0, Qt::WindowStaysOnTopHint);
    text = new QLabel(this);
    text->setText("LinQedIn v1.0 \nRealizzato da: Antonio Iacobucci \nProgetto didattico 2014-2015");
    text->setAlignment(Qt::AlignCenter);
    QFont aboutFont("Arial", 10, QFont::Bold);
    text->setFont(aboutFont);
    about->resize(300, 150);
    confirm= new QPushButton(tr("Chiudi"));
    confirm->setFixedSize(QSize(70,25));
    QVBoxLayout* lay = new QVBoxLayout();
    lay->addWidget(text);
    lay->addWidget(confirm,0, Qt::AlignCenter);
    about->setLayout(lay);
    about->show();
    connect(confirm, SIGNAL(clicked()), about, SLOT(close()));
}

void Widget::createMenu() {
    menuBar= new QMenuBar;
    fileMenu = menuBar->addMenu(tr("&File"));
    logAct= fileMenu->addAction(tr("&Login"));
    logAct->setStatusTip(tr("Autentica il tuo account"));
    regAct= fileMenu->addAction(tr("&Register"));
    regAct->setStatusTip(tr("Crea nuovo account"));
    fileMenu->addSeparator();
    exitAct = fileMenu->addAction(tr("&Exit"));
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Esci dall'applicazione"));
    viewMenu = menuBar->addMenu(tr("&View"));
    fullScreenAct = viewMenu->addAction(tr("&Full screen"));
    fullScreenAct->setCheckable(true);
    if(!(fullScreenAct->isChecked())){
        fullScreenAct->setStatusTip(tr("Visualizzazione full screen"));
    }
    else{
        fullScreenAct->setStatusTip(tr("Visualizzazione normale"));
    }
    searchMenu= menuBar->addMenu(tr("&Cerca"));
    searchAct= searchMenu->addAction(tr("&Cerca utente"));
    searchAct->setStatusTip(tr("Cerca profilo utente"));
    searchMenu->menuAction()->setVisible(false);
    helpMenu = menuBar->addMenu(tr("&Help"));
    aboutAct= helpMenu->addAction(tr("&About"));
    aboutAct->setStatusTip(tr("Info sull'applicazione"));
    mainLayout->setMenuBar(menuBar);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(logAct, SIGNAL(triggered()), this, SLOT(redirect()));
    connect(regAct, SIGNAL(triggered()), this, SLOT(redirect()));
    connect(searchAct, SIGNAL(triggered()), this, SLOT(redirect()));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(goAbout()));
    connect(fullScreenAct, SIGNAL(triggered()), this, SLOT(changeScreen()));
}

void Widget::createLogForm(){
    db->load(db->getPath());
    gBox = new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Sign In"));
    us=new QLabel(tr("&Username"));
    usedit = new QLineEdit;
    us->setBuddy(usedit);
    pw=new QLabel(tr("&Password"));
    pwedit = new QLineEdit;
    pwedit->setEchoMode(QLineEdit::Password);
    pw->setBuddy(pwedit);
    confirm= new QPushButton(tr("&Conferma"));
    confirm->setFixedSize(QSize(70,25));
    newreg=new QLabel(tr("Sei un nuovo utente?"));
    clickreg= new ClickableLabel(tr("Registrati ora"));
    clickreg->setStyleSheet("QLabel{color: blue;}");
    layout->addRow(us, usedit);
    layout->addRow(pw, pwedit);
    layout->addWidget(confirm);
    layout->addRow(newreg, clickreg);
    layout->setContentsMargins(100,30,100,0);
    gBox->setLayout(layout);
    mainLayout->addWidget(gBox);
    connect(confirm, SIGNAL(clicked()), this, SLOT(login()));
    connect(clickreg, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(clickreg, SIGNAL(hover()), this, SLOT(underline()));
}

void Widget::createRegForm(){
    gBox= new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Registra Nuovo Account"));
    us=new QLabel(tr("&Username"));
    useditR = new QLineEdit;
    us->setBuddy(useditR);
    pw=new QLabel(tr("&Password"));
    pweditR = new QLineEdit;
    pweditR->setEchoMode(QLineEdit::Password);
    pw->setBuddy(pweditR);
    pwc=new QLabel(tr("&Conferma password"));
    pwceditR = new QLineEdit;
    pwceditR->setEchoMode(QLineEdit::Password);
    pwc->setBuddy(pwceditR);
    confirm= new QPushButton(tr("&Conferma"));
    confirm->setFixedSize(QSize(70,25));
    layout->addRow(us, useditR);
    layout->addRow(pw, pweditR);
    layout->addRow(pwc, pwceditR);
    layout->addWidget(confirm);
    layout->setContentsMargins(100,30,100,0);
    gBox->setLayout(layout);
    mainLayout->addWidget(gBox);
    connect(confirm, SIGNAL(clicked()), this, SLOT(regAcc()));
}

void Widget::createModForm(){
    gBox = new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Modifica profilo"));
    us=new QLabel(tr("&Username"));
    useditP = new QLineEdit;
    useditP->setText(QString::fromStdString(utente->u->getUsername()));
    useditP->setReadOnly(true);
    useditP->setStyleSheet("QLineEdit { border : 1px solid;}");
    us->setBuddy(useditP);
    nm=new QLabel(tr("&Nome"));
    nmedit = new QLineEdit;
    nmedit->setText(QString::fromStdString(utente->u->getName()));
    nm->setBuddy(nmedit);
    sm=new QLabel(tr("&Cognome"));
    smedit = new QLineEdit;
    smedit->setText(QString::fromStdString(utente->u->getSurname()));
    sm->setBuddy(smedit);
    em=new QLabel(tr("&Email"));
    emedit = new QLineEdit;
    emedit->setText(QString::fromStdString(utente->u->getEmail()));
    em->setBuddy(emedit);
    cy=new QLabel(tr("&Citta"));
    cyedit = new QLineEdit;
    cyedit->setText(QString::fromStdString(utente->u->getCity()));
    cy->setBuddy(cyedit);
    sx=new QLabel(tr("&Sesso"));
    sxmod = new QComboBox;
    sxmod->addItem("-");
    sxmod->addItem("M");
    sxmod->addItem("F");
    QString sxdefault= QString::fromStdString(utente->u->getSesso());
    if(sxdefault == "M"){
        sxmod->setCurrentIndex(1);
    }
    else if(sxdefault == "F"){
        sxmod->setCurrentIndex(2);
    }
    else sxmod->setCurrentIndex(0);
    sx->setBuddy(sxmod);
    bh=new QLabel(tr("&Data di nascita"));
    bhedit = new QLineEdit;
    bhedit->setText(utente->u->getBirth().toString("yyyy'-'MM'-'dd"));
    bh->setBuddy(bhedit);
    qu=new QLabel(tr("&Qualifiche"));
    int qsize= utente->u->getQualification().size();
    qlw= new QListWidget;
    list<string> q_list= utente->u->getQualification();
    list<string>::iterator qiter = q_list.begin();
    for( ; qiter != q_list.end(); ++qiter){
        qlw->addItem(QString::fromStdString(*qiter));
    }
    qlw->setMaximumHeight(qlw->sizeHintForColumn(0));
    remQ= new QPushButton(tr("Rimuovi qualifica selezionata"));
    remQ->setFixedSize(QSize(200,25));
    remQ->setVisible(false);
    ac=new QLabel(tr("&Account"));
    acedit = new QLineEdit;
    acedit->setText(QString::fromStdString(utente->u->getAccount()));
    acedit->setReadOnly(true);
    acedit->setStyleSheet("QLineEdit { border : 1px solid;}");
    ac->setBuddy(acedit);
    addQ= new QPushButton(tr("Aggiungi qualifica"));
    addQ->setFixedSize(QSize(110,25));
    addQedit= new QLineEdit;
    addQedit->setVisible(false);
    confirm= new QPushButton(tr("&Conferma modifiche"));
    confirm->setFixedSize(QSize(110,25));
    pswP= new QPushButton(tr("&Cambia password"));
    pswP->setFixedSize(QSize(110,25));
    backP= new QPushButton(tr("&Indietro"));
    backP->setFixedSize(QSize(110,25));
    layout->addRow(us, useditP);
    layout->addRow(nm, nmedit);
    layout->addRow(sm, smedit);
    layout->addRow(em, emedit);
    layout->addRow(cy, cyedit);
    layout->addRow(sx, sxmod);
    layout->addRow(bh, bhedit);
    layout->addRow(ac, acedit);
    if(qsize==0){
        quedit= new QLineEdit;
        quedit->setReadOnly(true);
        qu->setBuddy(quedit);
        layout->addRow(qu, quedit);
    }
    else if(qsize>0){
        qu->setBuddy(qlw);
        layout->addRow(qu,qlw);
    }
    layout->addWidget(remQ);
    layout->addRow(addQ, addQedit);
    layout->addRow(pswP,new QWidget);
    layout->addRow(backP, confirm);
    layout->setContentsMargins(100,0,100,0);
    gBox->setLayout(layout);
    mainLayout->addWidget(gBox);
    connect(qlw, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(redirect()));
    connect(remQ, SIGNAL(clicked()), this, SLOT(removeQ()));
    connect(addQ, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(confirm, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(pswP, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(backP, SIGNAL(clicked()), this, SLOT(redirect()));
}

void Widget::createView(){
    createMenu();
    createLogForm();
}

void Widget::createAuthView(){
    clearLayout(layout);
    gBox = new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Profilo personale"));
    us=new QLabel(tr("&Username"));
    useditP = new QLineEdit;
    useditP->setText(QString::fromStdString(utente->u->getUsername()));
    useditP->setReadOnly(true);
    us->setBuddy(useditP);
    nm=new QLabel(tr("&Nome"));
    nmedit = new QLineEdit;
    nmedit->setText(QString::fromStdString(utente->u->getName()));
    nmedit->setReadOnly(true);
    nm->setBuddy(nmedit);
    sm=new QLabel(tr("&Cognome"));
    smedit = new QLineEdit;
    smedit->setText(QString::fromStdString(utente->u->getSurname()));
    smedit->setReadOnly(true);
    sm->setBuddy(smedit);
    em=new QLabel(tr("&Email"));
    emedit = new QLineEdit;
    emedit->setText(QString::fromStdString(utente->u->getEmail()));
    emedit->setReadOnly(true);
    em->setBuddy(emedit);
    cy=new QLabel(tr("&Citta"));
    cyedit = new QLineEdit;
    cyedit->setText(QString::fromStdString(utente->u->getCity()));
    cyedit->setReadOnly(true);
    cy->setBuddy(cyedit);
    sx=new QLabel(tr("&Sesso"));
    sxedit = new QLineEdit;
    sxedit->setText(QString::fromStdString(utente->u->getSesso()));
    sxedit->setReadOnly(true);
    sx->setBuddy(sxedit);
    bh=new QLabel(tr("&Data di nascita"));
    bhedit = new QLineEdit;
    bhedit->setText(utente->u->getBirth().toString("yyyy'-'MM'-'dd"));
    bhedit->setReadOnly(true);
    bh->setBuddy(bhedit);
    qu=new QLabel(tr("&Qualifiche"));
    int qsize= utente->u->getQualification().size();
    qlw= new QListWidget;
    list<string> q_list=utente->u->getQualification();
    list<string>::iterator qiter = q_list.begin();
    for( ; qiter != q_list.end(); ++qiter){
        qlw->addItem(QString::fromStdString(*qiter));
    }
    qlw->setMaximumHeight(qlw->sizeHintForColumn(0));
    ac=new QLabel(tr("&Account"));
    acedit = new QLineEdit;
    acedit->setText(QString::fromStdString(utente->u->getAccount()));
    acedit->setReadOnly(true);
    ac->setBuddy(acedit);
    modP= new QPushButton(tr("&Modifica Profilo"));
    modP->setFixedSize(QSize(90,25));
    contP= new QPushButton(tr("&Visualizza contatti"));
    contP->setFixedSize(QSize(110,25));
    layout->addRow(us, useditP);
    layout->addRow(nm, nmedit);
    layout->addRow(sm, smedit);
    layout->addRow(em, emedit);
    layout->addRow(cy, cyedit);
    layout->addRow(sx, sxedit);
    layout->addRow(bh, bhedit);
    layout->addRow(ac, acedit);
    if(qsize==0){
        quedit= new QLineEdit;
        quedit->setReadOnly(true);
        qu->setBuddy(quedit);
        layout->addRow(qu, quedit);
    }
    if(qsize>0){
        qu->setBuddy(qlw);
        layout->addRow(qu,qlw);
    }
    layout->addRow(modP, contP);
    layout->setContentsMargins(100,30,100,0);
    gBox->setLayout(layout);
    mainLayout->addWidget(gBox);
    connect(modP, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(contP, SIGNAL(clicked()), this, SLOT(redirect()));
}

void Widget::createAdminView(){
    clearLayout(layout);
    gBox = new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Area amministratore"));
    butt_change= new QPushButton(tr("Cambia tipologia account"));
    butt_change->setFixedSize(QSize(150,25));
    user=new QLabel(tr("&Username utente"));
    user->setVisible(false);
    useredit = new QLineEdit;
    useredit->setVisible(false);
    user->setBuddy(useredit);
    sub=new QLabel(tr("&Tipo account"));
    sub->setVisible(false);
    subedit = new QLineEdit;
    subedit->setVisible(false);
    sub->setBuddy(subedit);
    confirm= new QPushButton(tr("Conferma"));
    confirm->setFixedSize(QSize(70,25));
    confirm->setVisible(false);
    insert= new QPushButton(tr("Conferma"));
    insert->setFixedSize(QSize(70,25));
    insert->setVisible(false);
    addU= new QPushButton(tr("Inserisci utente"));
    addU->setFixedSize(QSize(110,25));
    remU= new QPushButton(tr("Rimuovi utente"));
    remU->setFixedSize(QSize(110,25));
    remUedit= new QLineEdit;
    searchbutton= new QPushButton(tr("Cerca utente"));
    searchbutton->setFixedSize(QSize(110,25));
    searchedit= new QLineEdit;
    us=new QLabel(tr("&Username"));
    us->setVisible(false);
    usedit = new QLineEdit;
    usedit->setVisible(false);
    us->setBuddy(usedit);
    pw=new QLabel(tr("&Password"));
    pw->setVisible(false);
    pwedit = new QLineEdit;
    pwedit->setVisible(false);
    pw->setBuddy(pwedit);
    ac=new QLabel(tr("&Tipo account"));
    ac->setVisible(false);
    acedit = new QLineEdit;
    acedit->setVisible(false);
    ac->setBuddy(acedit);
    layout->addRow(addU, new QWidget);
    layout->addRow(us, usedit);
    layout->addRow(pw, pwedit);
    layout->addRow(ac, acedit);
    layout->addWidget(insert);
    layout->addRow(remU, remUedit);
    layout->addRow(searchbutton, searchedit);
    layout->addRow(butt_change, new QWidget);
    layout->addRow(user,useredit);
    layout->addRow(sub,subedit);
    layout->addWidget(confirm);
    layout->setContentsMargins(100,30,100,0);
    gBox->setLayout(layout);
    mainLayout->addWidget(gBox);
    connect(butt_change, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(confirm, SIGNAL(clicked()), this, SLOT(changeType()));
    connect(remU, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(addU, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(insert, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(searchbutton, SIGNAL(clicked()), this, SLOT(adminSearch()));
}

void Widget::createContView(){
    gBox = new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Lista Contatti"));
    list<Username*> n_list= utente->u->getRete();
    int nsize= n_list.size();
    if(nsize == 0){
        text= new QLabel;
        text->setText("Nessun contatto presente");
        text->setAlignment(Qt::AlignHCenter);
        QFont textFont("Arial", 10, QFont::Bold);
        text->setFont(textFont);
    }
    else if(nsize>0){
        nlw= new QListWidget;
        list<Username*>::iterator niter = n_list.begin();
        for( ; niter != n_list.end(); ++niter){
            nlw->addItem(QString::fromStdString((**niter).username));
        }
    }
    profC= new QPushButton(tr("Profilo contatto selezionato"));
    profC->setFixedSize(QSize(200,25));
    profC->setVisible(false);
    addC= new QPushButton(tr("Aggiungi contatto"));
    addC->setFixedSize(QSize(100,25));
    addCedit= new QLineEdit;
    addCedit->setVisible(false);
    remC= new QPushButton(tr("Rimuovi contatto"));
    remC->setFixedSize(QSize(100,25));
    remCedit= new QLineEdit;
    remCedit->setVisible(false);
    confCont= new QPushButton(tr("Conferma"));
    confCont->setFixedSize(QSize(70,25));
    confCont->setVisible(false);
    backP= new QPushButton(tr("&Indietro"));
    backP->setFixedSize(QSize(100,25));
    if(nsize == 0){
        layout->addWidget(text);
        layout->addRow(addC, addCedit);
        layout->addRow(backP,confCont);
        layout->setContentsMargins(100,30,100,0);
        gBox->setLayout(layout);
        mainLayout->addWidget(gBox);
    }
    else if(nsize>0){
        layout->addWidget(nlw);
        layout->addWidget(profC);
        layout->addRow(addC, addCedit);
        layout->addRow(remC,remCedit);
        layout->addRow(backP,confCont);
        layout->setContentsMargins(100,30,100,0);
        gBox->setLayout(layout);
        mainLayout->addWidget(gBox);
        connect(nlw, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(redirect()));
        connect(profC,SIGNAL(clicked()), this, SLOT(redirect()));
        connect(remC, SIGNAL(clicked()), this, SLOT(redirect()));
    }
    connect(addC, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(confCont, SIGNAL(clicked()), this, SLOT(redirect()));
    connect(backP, SIGNAL(clicked()), this, SLOT(redirect()));
}

void Widget::createPswForm(){
    gBox = new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Cambio password"));
    old=new QLabel(tr("&Vecchia password"));
    oldedit = new QLineEdit;
    oldedit->setEchoMode(QLineEdit::Password);
    old->setBuddy(oldedit);
    nuova=new QLabel(tr("&Nuova password"));
    newedit = new QLineEdit;
    newedit->setEchoMode(QLineEdit::Password);
    nuova->setBuddy(newedit);
    newc=new QLabel(tr("&Conferma password"));
    newcedit = new QLineEdit;
    newcedit->setEchoMode(QLineEdit::Password);
    newc->setBuddy(newcedit);
    confirm= new QPushButton(tr("&Conferma"));
    confirm->setFixedSize(QSize(70,25));
    backP= new QPushButton(tr("&Indietro"));
    backP->setFixedSize(QSize(70,25));
    layout->addRow(old, oldedit);
    layout->addRow(nuova, newedit);
    layout->addRow(newc, newcedit);
    layout->addRow(backP, confirm);
    layout->setContentsMargins(100,30,100,0);
    gBox->setLayout(layout);
    mainLayout->addWidget(gBox);
    connect(confirm, SIGNAL(clicked()), this, SLOT(changePsw()));
    connect(backP, SIGNAL(clicked()), this, SLOT(redirect()));
}

void Widget::createSearchForm(){
    gBox = new QGroupBox;
    layout = new QFormLayout;
    gBox->setTitle(tr("Cerca Utente"));
    searchedit = new QLineEdit;
    searchedit->setFixedSize(400, 20);
    searchbutton= new QPushButton(tr("&Cerca"));
    searchbutton->setFixedSize(QSize(70,25));
    backP= new QPushButton(tr("&Indietro"));
    backP->setFixedSize(QSize(70,25));
    layout->addRow(searchedit, searchbutton);
    layout->addRow(backP, new QWidget);
    layout->setContentsMargins(100,30,100,0);
    gBox->setLayout(layout);
    mainLayout->addWidget(gBox);
    connect(searchbutton, SIGNAL(clicked()), this, SLOT(searchUser()));
    connect(backP, SIGNAL(clicked()), this, SLOT(redirect()));
}

void Widget::error(int e){
    switch(e){
        case(0): statusBar()->showMessage(tr("Username inesistente"));
                 usedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 pwedit->setStyleSheet("QLineEdit { border : 1px;}");
                 break;
        case(1): statusBar()->showMessage(tr("Errore Password"));
                 usedit->setStyleSheet("QLineEdit { border : 1px;}");
                 pwedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 break;
        case(2): statusBar()->showMessage(tr("Username non disponibile"));
                 useditR->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 pweditR->setStyleSheet("QLineEdit { border : 1px;}");
                 pwceditR->setStyleSheet("QLineEdit { border : 1px;}");
                 break;
        case(3): statusBar()->showMessage(tr("La password deve contenere almeno 6 caratteri"));
                 useditR->setStyleSheet("QLineEdit { border : 1px;}");
                 pweditR->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 pwceditR->setStyleSheet("QLineEdit { border : 1px;}");
                 break;
        case(4): statusBar()->showMessage(tr("Le password non coincidono"));
                 useditR->setStyleSheet("QLineEdit { border : 1px;}");
                 pweditR->setStyleSheet("QLineEdit { border : 1px;}");
                 pwceditR->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 break;
        case(5): statusBar()->showMessage(tr("Lo username deve contenere almeno 4 caratteri"));
                 useditR->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 pweditR->setStyleSheet("QLineEdit { border : 1px;}");
                 pwceditR->setStyleSheet("QLineEdit { border : 1px;}");
                 break;
        case(6): statusBar()->showMessage(tr("Errore vecchia password"));
                 newedit->setStyleSheet("QLineEdit { border : 1px;}");
                 newcedit->setStyleSheet("QLineEdit { border : 1px;}");
                 oldedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 break;
        case(7): statusBar()->showMessage(tr("La nuova password deve contenere almeno 6 caratteri"));
                 oldedit->setStyleSheet("QLineEdit { border : 1px;}");
                 newedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 newcedit->setStyleSheet("QLineEdit { border : 1px;}");
                 break;
        case(8): statusBar()->showMessage(tr("Le password non coincidono"));
                 oldedit->setStyleSheet("QLineEdit { border : 1px;}");
                 newedit->setStyleSheet("QLineEdit { border : 1px;}");
                 newcedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 break;
        case(9): statusBar()->showMessage(tr("Username inesistente"));
                 useredit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                 subedit->setStyleSheet("QLineEdit { border : 1px;}");
                 break;
        case(10): statusBar()->showMessage(tr("Tipologia account scorretta"));
                  useredit->setStyleSheet("QLineEdit { border : 1px;}");
                  subedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  break;
        case(11): statusBar()->showMessage(tr("Non puoi aggiungere te stesso"));
                  addCedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  break;
        case(12): statusBar()->showMessage(tr("Contatto inesistente"));
                  addCedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  break;
        case(13): statusBar()->showMessage(tr("Contatto inesistente"));
                  remCedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  break;
        case(14): statusBar()->showMessage(tr("Operazione eseguita"));
                  break;
        case(15): statusBar()->showMessage(tr("Contatto presente"));
                  addCedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  break;
        case(16): statusBar()->showMessage(tr("Utente inesistente"));
                  searchedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  break;
        case(17): statusBar()->showMessage(tr("Tipologia account scorretta"));
                  usedit->setStyleSheet("QLineEdit { border : 1px;}");
                  pwedit->setStyleSheet("QLineEdit { border : 1px;}");
                  acedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  break;
        case(18): statusBar()->showMessage(tr("Username non disponibile"));
                  usedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  pwedit->setStyleSheet("QLineEdit { border : 1px;}");
                  acedit->setStyleSheet("QLineEdit { border : 1px;}");
                  break;
        case(19): statusBar()->showMessage(tr("La password deve contenere almeno 6 caratteri"));
                  usedit->setStyleSheet("QLineEdit { border : 1px;}");
                  pwedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  acedit->setStyleSheet("QLineEdit { border : 1px;}");
                  break;
        case(20): statusBar()->showMessage(tr("Lo username deve contenere almeno 4 caratteri"));
                  usedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  pwedit->setStyleSheet("QLineEdit { border : 1px;}");
                  acedit->setStyleSheet("QLineEdit { border : 1px;}");
                  break;
        case(21): statusBar()->showMessage(tr("Utente inserito"));
                  break;
        case(22): statusBar()->showMessage(tr("Tipo uguale a quello attuale"));
                  useredit->setStyleSheet("QLineEdit { border : 1px;}");
                  subedit->setStyleSheet("QLineEdit { border : 2px solid red;}");
                  break;
    }
}

void Widget::insertC(string s){
    bool trovato= false, presente= false;
    if(utente->u->getUsername() == s) emit db->emit_err(11);
    else if (utente->u->getUsername() != s){
        list<SmartUtente*>::iterator user = db->db.begin();
        for( ; user!=db->db.end(); ++user ){
            if(!trovato){
                if((**user)->getUsername() == s){
                    trovato=true;
                    list<Username*> n_list=utente->u->getRete();
                    list<Username*>::iterator cont = n_list.begin();
                    for( ; cont!=n_list.end(); ++cont ){
                        if((**cont).username == s){
                            presente= true;
                            emit db->emit_err(15);
                        }
                    }
                    if(presente==false){
                        Username* us= new Username(s);
                        utente->insertRete(us);
                        db->save();
                        statusBar()->showMessage(tr("Contatto aggiunto"));
                    }
                }
            }
        }
        if (trovato==false) emit db->emit_err(12);
    }
}

void Widget::removeC(string s){
    bool trovato= false;
    if (utente->u->getUsername() != s){
        list<Username*> n_list= utente->u->getRete();
        list<Username*>::iterator cont = n_list.begin();
        for( ; cont!=n_list.end(); ++cont ){
            if((**cont).username == s){
                trovato=true;
                utente->removeRete(*cont);
                db->save();
                statusBar()->showMessage(tr("Contatto rimosso"));
            }
        }
        if (!trovato) emit db->emit_err(13);
    }
}


void Widget::underline(){
    clickreg->setStyleSheet("QLabel{color: blue; text-decoration: underline;}");
}
