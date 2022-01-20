#include "menuliftclickonly.h"
#include <QMouseEvent>
MenuLiftClickOnly::MenuLiftClickOnly(QWidget *parent)
    : QMenu{parent}
{

}

void MenuLiftClickOnly::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QMenu::mousePressEvent(event);
    }
}
