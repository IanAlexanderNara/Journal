#include "journalencrypter.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/conf.h>
#include <openssl/kdf.h>
#include <openssl/err.h>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
bool JournalEncrypter::prepareAndEncrypt(QString entryText, QString password, int date, QSqlDatabase db, QWidget *parent)
{
    // Get encryption paramters (password, plaintext, salt, key, iv)
    QByteArray pwba = password.toUtf8();
    const char *pw = pwba.constData();
    QByteArray ptba = entryText.toUtf8();
    const char *plaintext = ptba.constData();
    unsigned char salt[SALT_SIZE];
    if(RAND_bytes(salt, SALT_SIZE) != 1){
        QMessageBox::warning(parent, "Encryption Failed", "Couldn't get salt for password based encryption.");
        return false;
    }
    unsigned char key[KEY_SIZE];
    if(PKCS5_PBKDF2_HMAC(pw, pwba.size(), salt, SALT_SIZE, 10000, EVP_sha256(), KEY_SIZE, key) != 1){
        QMessageBox::warning(parent, "Encryption Failed", "Key derivation failed for password based encryption.");
        return false;
    }
    unsigned char iv[IV_SIZE];
    if(RAND_bytes(iv, IV_SIZE) != 1){
        QMessageBox::warning(parent, "Encryption Failed", "Couln't get iv for password based encryption.");
        return false;
    }

    // get ciphertext
    unsigned char* ciphertext = new unsigned char[ptba.size()+BLOCK_SIZE];
    int ctlen = encrypt((unsigned char *) plaintext, ptba.size(), key, iv, ciphertext, parent);
    if(ctlen == -1)
        return false;

    qDebug() << "salt: " << salt[0] << " " << salt[1] << "..." << salt[1022] << " " << salt[1023];
    qDebug() << "encrypt key: " << key[0] << " " << key[1] << "..." << key[14] << " " << key[15];
    qDebug() << "iv: " << iv[0] << " " << iv[1] << "..." << iv[14] << " " << iv[15];
    qDebug() << "ct: " << ciphertext[0] << " " << ciphertext[1] << "..." << ciphertext[ctlen-2] << ciphertext[ctlen-1];

    // mutate ciphertext and encyrption paramaters for database insertion
    QByteArray ctBytes((char *) ciphertext, ctlen);
    QByteArray saltBytes((char *) salt, SALT_SIZE);
    QByteArray ivBytes((char *) iv, IV_SIZE);

    // Insert into database
    QSqlQuery query(db);
    query.prepare("INSERT OR REPLACE INTO journal (date, entry, salt, iv) "
                  "VALUES (?, ?, ?, ?)");
    query.addBindValue(date);
    query.addBindValue(ctBytes);
    query.addBindValue(saltBytes);
    query.addBindValue(ivBytes);
    if(!query.exec()){
        QMessageBox::warning(parent, "Save Failed", "Failed to save journal entry.");
        return false;
    }
    return true;
}

QString JournalEncrypter::prepareAndDecrypt(QString password, int date, QSqlDatabase db, QWidget *parent)
{

    //Query the database
    QSqlQuery query(db);
    query.prepare("SELECT date, entry, salt, iv FROM journal WHERE date = ?");
    query.addBindValue(date);
    query.exec();
    query.next();
    if(!query.isValid()){
        QMessageBox::information(parent, "No Entry", "No entry found for specified date.");
        return nullptr;
    }

    // Get encryption paramters from query
    QByteArray ctba = query.value(1).toByteArray();
    const char *ciphertext = ctba.constData();
    QByteArray saltba = query.value(2).toByteArray();
    const char *salt = saltba.constData();
    QByteArray ivba = query.value(3).toByteArray();
    const char *iv = ivba.constData();
    QByteArray pwba = password.toUtf8();
    const char *pw = pwba.constData();

    // Derive encryption key
    unsigned char key[KEY_SIZE];
    if(PKCS5_PBKDF2_HMAC(pw, pwba.size(), (unsigned char *) salt, SALT_SIZE, 10000, EVP_sha256(), KEY_SIZE, key) != 1){
        QMessageBox::warning(parent, "Encryption Failed", "Key derivation failed for password based encryption.");
        return nullptr;
    }

    // Get plaintext
    unsigned char* plaintext = new unsigned char[ctba.size()];
    int ptlen = decrypt((unsigned char *) ciphertext, ctba.size(), key, (unsigned char *) iv, plaintext, parent);
    if(ptlen == -1)
        return nullptr;
    else if(ptlen == -2){
        // if failed to decrypt because password incorrect, notify by empty string
        // (empty string is never allowed to be submiteed to db as an entry)
        return QString("");
    }

    qDebug() << "salt: " << (unsigned char ) salt[0] << " " << (unsigned char )salt[1] << "..." << (unsigned char )salt[1022] << " " << (unsigned char )salt[1023];
    qDebug() << "encrypt key: " << (unsigned char )key[0] << " " << (unsigned char )key[1] << "..." << (unsigned char )key[14] << " " << (unsigned char )key[15];
    qDebug() << "iv: " << (unsigned char )iv[0] << " " << (unsigned char )iv[1] << "..." << (unsigned char )iv[14] << " " << (unsigned char )iv[15];
    qDebug() << "ct: " << (unsigned char )ciphertext[0] << " " << (unsigned char )ciphertext[1] << "..." << (unsigned char )ciphertext[ctba.size()-2] << (unsigned char )ciphertext[ctba.size()-1];

    return QString::fromUtf8((const char *) plaintext, ptlen);
}

int JournalEncrypter::encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext,
                              QWidget *parent)
{
    // init
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;
    if(!(ctx = EVP_CIPHER_CTX_new())){
        QMessageBox::warning(parent, "Encryption Failed", "Encryption failure in initialization.");
        return -1;
    }
    if(1!= EVP_EncryptInit_ex(ctx,EVP_aes_128_cbc(), NULL, key, iv)){
        QMessageBox::warning(parent, "Encryption Failed", "Encryption failure in initialization.");
        return -1;
    }
    //encrypt
    if(1!= EVP_EncryptUpdate(ctx,ciphertext, &len, plaintext, plaintext_len)){
        QMessageBox::warning(parent, "Encryption Failed", "Encryption Failed");
        return -1;
    }
    ciphertext_len = len;
    if(1!= EVP_EncryptFinal_ex(ctx,ciphertext+len, &len)){
        QMessageBox::warning(parent, "Encryption Failed", "Encryption Failed");
        return -1;
    }
    //return length
    ciphertext_len+=len;
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int JournalEncrypter::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext,
                              QWidget *parent)
{
    //init
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    if(!(ctx= EVP_CIPHER_CTX_new())){
        QMessageBox::warning(parent, "Decryption Failed", "Decryption failure in initialization.");
        return -1;
    }
    if(1!= EVP_DecryptInit_ex(ctx,EVP_aes_128_cbc(), NULL, key, iv)){
        QMessageBox::warning(parent, "Decryption Failed", "Decryption failure in initialization.");
        return -1;
    }
    //decrypt
    if(1!= EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)){
        QMessageBox::warning(parent, "Decryption Failed", "Decryption Failed");
        return -1;
    }
    plaintext_len = len;
    if(1!= EVP_DecryptFinal_ex(ctx, plaintext+len, &len)){
        // If failted by password being wrong notify by -2
        return -2;
    }
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}
