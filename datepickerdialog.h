#ifndef DATEPICKERDIALOG_H
#define DATEPICKERDIALOG_H

#include <QDialog>

namespace Ui {
class DatePickerDialog;
}

class DatePickerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatePickerDialog(QWidget *parent = nullptr);
    ~DatePickerDialog();

private:
    Ui::DatePickerDialog *ui;
};

#endif // DATEPICKERDIALOG_H
