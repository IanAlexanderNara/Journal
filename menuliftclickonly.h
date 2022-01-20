#ifndef MENULIFTCLICKONLY_H
#define MENULIFTCLICKONLY_H

#include <QMenu>

class MenuLiftClickOnly : public QMenu
{
    Q_OBJECT
public:
    explicit MenuLiftClickOnly(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // MENULIFTCLICKONLY_H
