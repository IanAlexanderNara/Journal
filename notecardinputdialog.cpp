#include "notecardinputdialog.h"
#include "ui_notecardinputdialog.h"

NotecardInputDialog::NotecardInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotecardInputDialog)
{
    ui->setupUi(this);
    ui->textEdit->setFocus();
}

NotecardInputDialog::~NotecardInputDialog()
{
    delete ui;
}

QString NotecardInputDialog::getText()
{
    return ui->textEdit->toPlainText();
}

void NotecardInputDialog::setText(QString text)
{
    QStringList list = text.split(QRegExp("[\r\n]"), QString::KeepEmptyParts);
    for(int i = 0; i < list.size(); i++){
        ui->textEdit->append(list.at(i));
        ui->textEdit->setAlignment(Qt::AlignCenter);
    }
    ui->textEdit->setAlignment(Qt::AlignCenter);
}

void NotecardInputDialog::setFocus()
{
    ui->textEdit->setFocus();
}

void NotecardInputDialog::on_pushButton_clicked()
{
    this->close();
}

