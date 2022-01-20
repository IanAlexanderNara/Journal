#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include "journal.h"
namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void saveClicked();
    void calculateClicked();



private:
    const QString SETTINGS_PATH = QCoreApplication::applicationDirPath() + "/settings/settings.INI";

    Ui::SettingsDialog *ui;


};

#endif // SETTINGSDIALOG_H
