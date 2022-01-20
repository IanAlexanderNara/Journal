#include "mouseclickfilter.h"
#include <QEvent>
#include <QMouseEvent>
MouseClickFilter::MouseClickFilter(QObject *parent)
    : QObject{parent}
{

}

bool MouseClickFilter::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    // When the menu is clicked with anything other than the left button, ignore it
    // All other events are let through
    // This disables triggering menu item by right click, to easy to accidentally do.
    // (menu activated with the right button)
    if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->button() == Qt::LeftButton) return false;
        else return true;
    }
    return false;
}
