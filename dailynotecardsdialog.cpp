#include "dailynotecardsdialog.h"
#include "ui_dailynotecardsdialog.h"

DailyNotecardsDialog::DailyNotecardsDialog(QWidget *parent, QList<int> daysIds, QStringList daysAuthors, QStringList daysTitles, QStringList daysCards) :
    QDialog(parent),
    ui(new Ui::DailyNotecardsDialog)
{
    this->daysCards = daysCards;
    this->daysAuthors = daysAuthors;
    this->daysIds = daysIds;
    this->daysTitles = daysTitles;
    this->index = 0;
    ui->setupUi(this);
    if(!this->daysCards.empty()) ui->card->setText(this->daysCards.at(0));
    this->setWindowTitle(makeWindowTitle());
    QString labelText = QString::number(this->index+1) + "/" + QString::number(this->daysCards.size());
    ui->label->setText(labelText);
    this->setWindowTitle(makeWindowTitle());
}

DailyNotecardsDialog::~DailyNotecardsDialog()
{
    delete ui;
}

void DailyNotecardsDialog::on_nextButton_clicked()
{
    if(daysCards.empty() || this->index == daysCards.size()-1){
        return;
    }
    this->index++;
    ui->card->setText(this->daysCards.at(this->index));
    this->setWindowTitle(makeWindowTitle());
    QString labelText = QString::number(this->index+1) + "/" + QString::number(this->daysCards.size());
    ui->label->setText(labelText);
}


void DailyNotecardsDialog::on_previousButton_clicked()
{
    if(daysCards.empty() || this->index == 0){
        return;
    }
    this->index--;
    ui->card->setText(this->daysCards.at(this->index));
    this->setWindowTitle(makeWindowTitle());
    QString labelText = QString::number(this->index+1) + "/" + QString::number(this->daysCards.size());
    ui->label->setText(labelText);
}

QString DailyNotecardsDialog::makeWindowTitle(){
    if(this->daysAuthors.size() > this->index && this->daysTitles.size() > this->index && this->daysIds.size() > this->index) {
        QString r = QString::number(this->daysIds.at(this->index)) + ", " + this->daysAuthors.at(this->index) + " - " + this->daysTitles.at(this->index);
        return r;
    } else {
        return " ";
    }
}
