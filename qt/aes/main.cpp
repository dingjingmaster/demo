#include <QApplication>
#include <openssl/aes.h>
#include <glib.h>
#include <QDebug>

static const unsigned char PEONY_AES_KEY[] = "peony key";

static QString passwdEncode (QString& p)
{
    g_return_val_if_fail (!p.isEmpty (), "");

    unsigned char aesKey[AES_BLOCK_SIZE] = {0};
    unsigned char ivc[AES_BLOCK_SIZE] = {0};

    int passwdLength = p.toUtf8().size () + 1;
    if (passwdLength % AES_BLOCK_SIZE) passwdLength += (passwdLength % AES_BLOCK_SIZE);
    g_autofree unsigned char* tmp = (unsigned char*) g_malloc0 (passwdLength);

    memcpy (tmp, p.toUtf8 ().constData (), p.toUtf8 ().size ());

    int encslength = ((passwdLength + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    g_autofree unsigned char* encodeOut = (unsigned char*) g_malloc0 (encslength + 1);

    memset (ivc, 0, sizeof (ivc));
    AES_KEY encKey;
    memcpy (aesKey, PEONY_AES_KEY, sizeof (PEONY_AES_KEY));
    AES_set_encrypt_key (aesKey, 128, &encKey);
    AES_cbc_encrypt (tmp, encodeOut, p.toUtf8 ().size (), &encKey, ivc, AES_ENCRYPT);

    return QString("%1|%2").arg (p.toUtf8 ().size ()).arg (QString (QByteArray::fromRawData ((char*) encodeOut, encslength).toBase64 ()));
}

static QString passwdDecode (QString& p)
{
    g_return_val_if_fail (!p.isEmpty (), "");

    QStringList ls = p.split ("|");

    g_return_val_if_fail (ls.length () == 2, "");

    unsigned char aesKey[AES_BLOCK_SIZE] = {0};
    unsigned char ivc[AES_BLOCK_SIZE] = {0};

    int srcPasswdLength = ls.first ().toInt ();
    QByteArray data = QByteArray::fromBase64 (ls.last ().toUtf8 ());
    int encodePasswdLength = data.length ();

    if (encodePasswdLength % AES_BLOCK_SIZE) encodePasswdLength += (encodePasswdLength % AES_BLOCK_SIZE);
    g_autofree unsigned char* tmp = (unsigned char*) g_malloc0 (encodePasswdLength);

    memcpy (tmp, data.constData (), data.length ());

    g_autofree unsigned char* decodeOut = (unsigned char*) g_malloc0 (srcPasswdLength + 1);

    memset (ivc, 0, sizeof (ivc));
    memcpy (aesKey, PEONY_AES_KEY, sizeof (PEONY_AES_KEY));

    AES_KEY decKey;
    AES_set_decrypt_key (aesKey, 128, &decKey);
    AES_cbc_encrypt (tmp, decodeOut, srcPasswdLength, &decKey, ivc, AES_DECRYPT);

    return (char*) decodeOut;
}

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);

    QString str = "我是中国人!!!";

    QString encode = passwdEncode (str);
    qDebug () << "|" << str << " -- " << encode << "|";

    QString decode = passwdDecode (encode);
    qDebug () << "|" << encode << " -- " << (decode) << "|";


//    return a.exec();
}
