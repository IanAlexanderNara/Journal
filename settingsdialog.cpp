#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QKeyEvent>
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::saveClicked);
    connect(ui->buttonCalculate, &QPushButton::clicked, this, &SettingsDialog::calculateClicked);

    // populate date edits with the current settings
    // load the user's birth and death date for settings or create if doesn't exist
    QSettings settings(SETTINGS_PATH, QSettings::IniFormat, this);
    settings.beginGroup(Journal::BIRTH_DEATH_GROUP);
    QVariant b = settings.value(Journal::BIRTH_KEY, QVariant());
    QVariant d = settings.value(Journal::DEATH_KEY, QVariant());
    settings.endGroup();
    if(b.isValid()){
        QDateTime birthDate = b.toDateTime();
        ui->dateEditBirth->setDate(birthDate.date());
    }
    if(d.isValid()){
        QDateTime deathDate = d.toDateTime();
        ui->dateEditDeath->setDate(deathDate.date());
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;

}

void SettingsDialog::keyPressEvent(QKeyEvent *event)
{
    // do not allow the dialog to be closed when enter is pressed
    // (disable default behavior: if the user presses enter in one of the dateEdits, the dialog closes,
    // potentially before they were finished editing settings)
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        return;
    else {
        QDialog::keyPressEvent(event);
    }
}

void SettingsDialog::saveClicked()
{
    QDateTime birthDate = ui->dateEditBirth->date().startOfDay();
    QDateTime deathDate = ui->dateEditDeath->date().startOfDay();
    QSettings settings(SETTINGS_PATH, QSettings::IniFormat, this);
    settings.beginGroup(Journal::BIRTH_DEATH_GROUP);
    settings.setValue(Journal::BIRTH_KEY, birthDate);
    settings.setValue(Journal::DEATH_KEY, deathDate);
    settings.endGroup();
}

void SettingsDialog::calculateClicked()
{
    QDateTime birthDate = ui->dateEditBirth->date().startOfDay();
    QDateTime deathDate = Journal::calcDeathDate(birthDate, this);
    ui->dateEditDeath->setDate(deathDate.date());
}

