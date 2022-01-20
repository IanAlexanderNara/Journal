#ifndef JOURNALENCRYPTER_H
#define JOURNALENCRYPTER_H

#include <QWidget>
#include <QSqlDatabase>
class JournalEncrypter
{
public:
    static bool prepareAndEncrypt(QString entryText, QString password, int date, QSqlDatabase db, QWidget *parent);
    static QString prepareAndDecrypt(QString password, int date, QSqlDatabase db, QWidget *parent);
private:
    static const int KEY_SIZE = 16;
    static const int SALT_SIZE = 1024;
    static const int IV_SIZE = 16;
    static const int BLOCK_SIZE = 16;

    static int encrypt(unsigned char * plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext,
                       QWidget *parent);
    static int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char* key, unsigned char *iv, unsigned char * plaintext,
                       QWidget *parent);
};

#endif // JOURNALENCRYPTER_H
