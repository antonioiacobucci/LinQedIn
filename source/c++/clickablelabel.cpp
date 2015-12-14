#include <iostream>
#include "clickablelabel.h"

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent):QLabel(parent){
    this->setText(text);
}

ClickableLabel::~ClickableLabel(){}

void ClickableLabel::mousePressEvent(QMouseEvent* event){
    emit hover();
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event){
    emit clicked();
}
