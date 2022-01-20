#ifndef MOUSECLICKFILTER_H
#define MOUSECLICKFILTER_H

#include <QObject>

class MouseClickFilter : public QObject
{
    Q_OBJECT
public:
    explicit MouseClickFilter(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MOUSECLICKFILTER_H
