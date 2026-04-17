#include <QSslSocket>
#include <QDebug>

int main () 
{
    qDebug() << QSslSocket::supportsSsl();
    qDebug() << "SSL library version string: " << QSslSocket::sslLibraryVersionString();
    qDebug() << "SSL library version number: " << QSslSocket::sslLibraryVersionNumber();
    qDebug() << "Build       version string: " << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "Build       version number: " << QSslSocket::sslLibraryBuildVersionNumber();

    return 0;
}
