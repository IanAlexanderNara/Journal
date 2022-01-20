#include "journal.h"
#include "ui_journal.h"
#include "mainwindow.h"

const QString Journal::BIRTH_DEATH_GROUP = "BirthDeath";
const QString Journal::BIRTH_KEY = "Birth";
const QString Journal::DEATH_KEY = "Death";
const QString Journal::DATE_FORMAT = "MM/dd/yyyy";

Journal::Journal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Journal)
{
    ui->setupUi(this);

    // Setup basic signals/slots for the ui
    connect(ui->buttonSave, &QPushButton::clicked, this, &Journal::buttonSaveClicked);
    connect(ui->buttonLoad, &QPushButton::clicked, this, &Journal::buttonLoadClicked);
    connect(ui->checkBoxShowPassword, &QCheckBox::stateChanged, this, &Journal::showPasswordChanged);
    connect(ui->buttonFormat, &QPushButton::clicked, this, [=](){
        formatAsSeconds = !formatAsSeconds;
        updateTimer();
    });

    // Make date the current date
    ui->dateEdit->setDate(QDate::currentDate());

    // Setup context menu for list view
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &Journal::showContextMenuDates);

    // Create our journal database if it does not exist
    db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
    db.setDatabaseName(DB_PATH);
    if(!db.open()){
        qDebug() << db.lastError();
        QMessageBox::warning(this, "Fatal Error", "Could not establish connection to journal database.");
        exit(1);
    }
    QSqlQuery query(db);
    QString sqlString(QString("CREATE TABLE IF NOT EXISTS journal ") +
                      QString("(date INTERGER PRIMARY KEY, entry BLOB NOT NULL, salt BLOB NOT NULL, iv BLOB NOT NULL);"));
    query.exec(sqlString);

    // Setup the model for displaying journal entries in list view.
    model= new JournalSqlModel(this, db);
    model->setTable("journal");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, QString("Date"));
    ui->treeView->setModel(model);
    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->show();


    // Setup shortcut
    saveShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(saveShortcut, &QShortcut::activated, this, &Journal::buttonSaveClicked);

    // Setup list view selection changed and double clicked
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &Journal::treeViewSelectionChanged);
    connect(ui->treeView, &QAbstractItemView::doubleClicked, this, &Journal::treeViewDoubleClicked);

    // Initialize death countdown timer
    initBirthDeath();
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &Journal::updateTimer);
    timer->start();
    updateTimer();
}

Journal::~Journal()
{
    delete ui;
}

void Journal::buttonSaveClicked()
{
    // Collect and verify needed information from ui
    QString entryText = ui->textEdit->toPlainText();
    if(entryText.isEmpty()){
        emit sendStatusbarMessage("Entry is empty!");
        return;
    }
    QString password = ui->lineEditPassword->text();
    if(password.isEmpty()){
        password = QInputDialog::getText(this, "Enter Password", "Enter an encryption/decryption password.", QLineEdit::Password);
        if(password.isEmpty())
            return;
        ui->lineEditPassword->setText(password);
    }

    // make date into integer
    int date = ui->dateEdit->date().toJulianDay();



    // encrypt
    bool success = JournalEncrypter::prepareAndEncrypt(entryText, password, date, db, this);
    if(!success)
        return;

    // Notify user that the save completed successfully.
    emit sendStatusbarMessage(QString("Encryption successful! Entry has been saved."));

    // update model
    model->select();
}

void Journal::buttonLoadClicked()
{
    // Collect and verify needed information from ui
    QString password = ui->lineEditPassword->text();
    if(password.isEmpty()){
        password = QInputDialog::getText(this, "Enter Password", "Enter an encryption/decryption password.", QLineEdit::Password);
        if(password.isEmpty())
            return;
    }

    // get date as integer
    int date = ui->dateEdit->date().toJulianDay();

    // decrypt
    QString plaintext = JournalEncrypter::prepareAndDecrypt(password, date, db, this);
    if (plaintext == ""){
        // password incorrect
        QMessageBox::warning(this, "Decryption Failed", "Decryption Failed");
        return;
    } else if(plaintext == nullptr){
        // Other failure
        QMessageBox::warning(this, "Decryption Failed", "Decryption Failed");
        return;
    }

    // update password if successfully decrypted
    ui->lineEditPassword->setText(password);

    // update text edit
    ui->textEdit->setText(plaintext);


    // notify user decryption was successful.
    emit sendStatusbarMessage(QString("Decryption successful!"));
}



void Journal::showPasswordChanged(bool checked){
    if(checked)
        ui->lineEditPassword->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEditPassword->setEchoMode(QLineEdit::Password);
}

void Journal::showContextMenuDates(const QPoint &pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    // Return if user didn't click directly on an tiem
    if(index.row() == -1)
        return;
    // Get global position for pop#include "tests/testjournal.h"ping up menu
    QPoint globalPos = ui->treeView->mapToGlobal(pos);
    //Construct menu and actions
    QMenu menu;
    menu.installEventFilter(new MouseClickFilter(this));
    menu.addAction("Delete");
    QAction *action = menu.exec(globalPos);
    if(action && action->text() == "Delete"){
        QString date = model->data(index, Qt::DisplayRole).toString();
        QString message = QString("Are you sure you want to delete the entry from %1 ?").arg(date);
        if (QMessageBox::Yes == QMessageBox(QMessageBox::Question, "Delete Entry", message, QMessageBox::Yes|QMessageBox::No, this).exec()){
            int julianDate = QDate::fromString(date, QString(DATE_FORMAT)).toJulianDay();
            QSqlQuery query(db);
            query.prepare("DELETE FROM journal WHERE date = ?");
            query.addBindValue(julianDate);
            if(query.exec()){
                emit sendStatusbarMessage(QString("Deletion of entry from %1 successful.").arg(date));
            } else {
                QMessageBox::warning(this, "Deletion Failed", "Failed to delete entry from database.");
            }
            model->select();
        }
    }
}

void Journal::treeViewSelectionChanged()
{
    QModelIndexList selected = ui->treeView->selectionModel()->selectedIndexes();
    QModelIndex dateIndex;
    for(int i = 0; i < selected.size(); i++){
        if(selected[i].column() == 0) dateIndex = selected[i];
    }
    if(dateIndex.isValid()){
        QDate date = QDate::fromString(model->data(selected[0], Qt::DisplayRole).toString(), DATE_FORMAT);
        ui->dateEdit->setDate(date);
    }
}


void Journal::treeViewDoubleClicked(const QModelIndex &index)
{
    if(index.column() != 0)
        return;
    else {
        QDate date = QDate::fromString(index.data().toString(), DATE_FORMAT);
        ui->dateEdit->setDate(date);
        QString password = QInputDialog::getText(this, "Enter Password", "Enter an encryption/decryption password.", QLineEdit::Password);
        if(password.isEmpty())
            return;
        ui->lineEditPassword->setText(password);
        buttonLoadClicked();
    }

}


void Journal::initBirthDeath()
{
    // load the user's birth and death date for settings or create if doesn't exist
    QSettings settings(SETTINGS_PATH, QSettings::IniFormat, this);
    settings.beginGroup(BIRTH_DEATH_GROUP);
    QVariant b = settings.value(BIRTH_KEY, QVariant());
    if(b == QVariant()){
        // no birthday set, get birthday
        DatePicker *datePicker = new DatePicker(this);
        datePicker->setWindowTitle("Enter Your Birthday");
        datePicker->exec();
        birthDate = datePicker->getDate().startOfDay();
        settings.setValue(BIRTH_KEY, birthDate);
    } else {
        birthDate = b.toDateTime();
    }
    QVariant d = settings.value(DEATH_KEY, QVariant());
    if(d == QVariant()){
        // no death day set, calculate from birthday
        deathDate = calcDeathDate(birthDate, this);
        settings.setValue(DEATH_KEY, deathDate);
    } else {
        deathDate = d.toDateTime();
    }
    settings.endGroup();
}

QDateTime Journal::calcDeathDate(QDateTime birth, QWidget *parent)
{
    QMap<int, int> map;

    // load actuarial table into map
    QFile file(":/life_table/life_table.csv");
    if(!file.open(QIODevice::ReadOnly)){
        QString message = "Failed to load actuarial table, defaulting to death day of 2060. You may modify manually in settings.";
        QMessageBox::warning(parent, "Couldn't Open File", message);
        return QDate::fromString("01/01/2060", Journal::DATE_FORMAT).startOfDay();
    }

    QTextStream stream(&file);
    while(!stream.atEnd()){
        QString line = stream.readLine();
        QStringList pair = line.split(",");
        QString a = pair.value(0);
        if(a == QString()){
            QString message = "Failed to load actuarial table, defaulting to death day of 2060. You may modify manually in settings.";
            QMessageBox::warning(parent, "Couldn't Open File", message);
            return QDate::fromString("01/01/2060", Journal::DATE_FORMAT).startOfDay();
        }
        bool ok;
        int age =  a.toInt(&ok);
        if(!ok){
            QString message = "Failed to load actuarial table, defaulting to death day of 2060. You may modify manually in settings.";
            QMessageBox::warning(parent, "Couldn't Open File", message);
            return QDate::fromString("01/01/2060", Journal::DATE_FORMAT).startOfDay();
        }
        QString e = pair.value(1);
        if(e == QString()){
            QString message = "Failed to load actuarial table, defaulting to death day of 2060. You may modify manually in settings.";
            QMessageBox::warning(parent, "Couldn't Open File", message);
            return QDate::fromString("01/01/2060", Journal::DATE_FORMAT).startOfDay();
        }
        int expectancy = e.toInt();
        map.insert(age, expectancy);
    }

    // calculate the users age based on their birthday
    int age = QDate::currentDate().year() - birth.date().year();
    if(QDate::currentDate().month() < birth.date().month()){
        age--;
    } else if(QDate::currentDate().month() == birth.date().month()){
        if(QDate::currentDate().day() < birth.date().day()){
            age--;
        }
    }
    if(age < 0)
        age = 0;
    else if(age > 119)
        age = 119;
    int remainingYears = map[age];
    QDateTime death = (QDate::currentDate().addYears(remainingYears)).startOfDay();
    return death;
}

void Journal::updateTimer()
{
    long long int msPassed = birthDate.msecsTo(QDateTime::currentDateTime());
    QString timePassed = millisecondsToTime(msPassed);
    ui->timePassed->setText(timePassed);

    long long int msRemaining = QDateTime::currentDateTime().msecsTo(deathDate);
    QString timeRemaining = millisecondsToTime(msRemaining);
    ui->timeRemaining->setText(timeRemaining);


}

QString Journal::millisecondsToTime(long long int milliseconds){
    long long int seconds = milliseconds / 1000;
    if(formatAsSeconds){
        return QString::number(seconds);
    }
    long long int minutes= seconds / 60;
    seconds = seconds % 60;
    long int hours = minutes / 60;
    minutes = minutes % 60;
    long int days= hours / 24;
    hours = hours % 24;

    QString time = QString::number(days);
    QString hrs = QString::number(hours);
    if(hrs.length() == 1){
        hrs = QString("0") + hrs;
    }
    time = time + "D:" + hrs;
    QString mins = QString::number(minutes);
    if(mins.length() == 1){
        mins = QString("0") + mins;
    }
    time= time + "H:" + mins;
    QString secs = QString::number(seconds);
    if(secs.length() == 1){
        secs = QString("0") + secs;
    }
    time = time + "M:" + secs + "s";
    return time;
}
