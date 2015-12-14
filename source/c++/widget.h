#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QMenu>
#include <QGroupBox>
#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QLayout>
#include <QFormLayout>
#include <QTextLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QString>
#include <client.h>
#include <admin.h>
#include <clickablelabel.h>
#include <QListWidget>
#include "database.h"
#include <string>


class Widget : public QMainWindow {
    Q_OBJECT
public:
    Widget();
public slots:
    void searchUser();          //ricerca utente
    void adminSearch();         //ricerca e visualizza utente da admin
    void removeQ();             //rimuovi qualifica
    void underline();           //sottolinea testo
    void redirect();            //dirige il redirect delle pagine, la visibilità di bottoni lineedit
                                //e la gestione di alcuni slot minori a seconda del sender() del SIGNAL
    void goAbout();             //visualizza about
    void changeScreen();        //fullscreen
    void login();               //login
    void regAcc();              //registrazione
    void changePsw();           //cambio password
    void error(int e);          //gestisce la segnalazione di messaggi, errori e visualizzazione delle caselle di testo errate
private slots:
    void changeType();          //cambio tipologia account
private:
    Client* utente;
    Admin* amministratore;
    Database* db;
    bool loggedC;               //variabile client autenticato
    bool loggedA;               //variabile admin autenticato
    //componenti grafiche
    QVBoxLayout* mainLayout;
    QMenuBar * menuBar;
    QMenu* fileMenu, *viewMenu, *searchMenu, *helpMenu;
    QGroupBox* gBox;
    QAction* aboutAct, *logAct, *regAct, *exitAct, *fullScreenAct, *searchAct;
    QFormLayout* layout;
    QLabel* us, *pw, *newreg, *pwc, *nm, *sm, *em, *cy, *sx, *bh, *qu, *ac, *cu, *old, *nuova, *newc, *text, *sub, *user;
    QDialog *about;
    QLineEdit* usedit, *pwedit, *useditR, *pweditR, *pwceditR, *useditP, *nmedit, *smedit, *emedit, *cyedit, *sxedit, *bhedit, *quedit, *acedit, *cuedit, *oldedit, *newedit, *newcedit, *addQedit, *addCedit, *remCedit, *subedit, *useredit, *searchedit, *remUedit;
    QComboBox* sxmod;
    QPushButton* confirm, *insert, *modP, *contP, *backP, *backA, *pswP, *addQ, *remQ, *addC, *remC, *addU, *remU, *confCont, *butt_change, *profC, *searchbutton;
    ClickableLabel* clickreg;
    QListWidget* qlw, *nlw;
    //metodi widget
    void insertC(string s);         //inserisci account
    void removeC(string s);         //rimuovi account
    void logout();                  //logout
    void editInfo();                //modifica informazioni
    void editNet();                 //modifica rete
    void viewUser(Utente* u);       //visualizza profilo utente cercato
    void viewCont(Username c);      //visualizza profilo contatto
    void clearLayout(QLayout* l);   //pulisce layout
    void createView();              //crea layout dell'avvio applicazione
    void createAuthView();          //crea layout utente autenticato
    void createAdminView();         //crea leyout admin autenticato
    void createContView();          //crea layout visualizzazione rete
    void createMenu();              //crea barra del menu
    void createLogForm();           //crea layout login
    void createRegForm();           //crea layout registrazione
    void createModForm();           //crea layout modifica profilo
    void createPswForm();           //crea layout modifica password
    void createSearchForm();        //crea layout ricerca
};

#endif // WIDGET_H
