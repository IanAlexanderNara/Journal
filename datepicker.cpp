#include "datepicker.h"
#include "ui_datepicker.h"
#include <QKeyEvent>
DatePicker::DatePicker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatePicker)
{
    ui->setupUi(this);
    ui->calendarWidget->setFocus();
}

DatePicker::~DatePicker()
{
    delete ui;
}

QDate DatePicker::getDate()
{
    return ui->calendarWidget->selectedDate();
}

void DatePicker::keyPressEvent(QKeyEvent *event)
{
    // do not allow the dialog to be closed when enter is pressed
    // (if the user presses enter to set the year in the calendar, the dialog closes,
    // potentially before they were finished choosing the date)
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        return;
    else {
        QDialog::keyPressEvent(event);
    }
}
