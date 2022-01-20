#ifndef DATEPICKER_H
#define DATEPICKER_H

#include <QDialog>

namespace Ui {
class DatePicker;
}

class DatePicker : public QDialog
{
    Q_OBJECT

public:
    explicit DatePicker(QWidget *parent = nullptr);
    ~DatePicker();
    QDate getDate();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::DatePicker *ui;


};

#endif // DATEPICKER_H
