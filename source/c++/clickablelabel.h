#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include <QLabel>
#include <QTextEdit>

class ClickableLabel: public QLabel{
    Q_OBJECT
public:
    explicit ClickableLabel( const QString& text ="", QWidget* parent=0);
    ~ClickableLabel();
signals:
    void clicked();
    void hover();
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CLICKABLELABEL_H
