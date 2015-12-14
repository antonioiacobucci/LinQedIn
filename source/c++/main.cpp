#include "utente.h"
#include <QApplication>
#include <QPushButton>
#include <iostream>
#include <QMainWindow>
#include <QWidget>
#include "Widget.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Widget widget;
    widget.show();
    return app.exec();
}
