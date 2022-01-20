#include "datepickerdialog.h"
#include "ui_datepickerdialog.h"

DatePickerDialog::DatePickerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatePickerDialog)
{
    ui->setupUi(this);
}

DatePickerDialog::~DatePickerDialog()
{
    delete ui;
}
