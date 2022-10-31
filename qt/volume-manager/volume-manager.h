#ifndef VOLUMEMANAGER_H
#define VOLUMEMANAGER_H

#include <QObject>
#include <qglobal.h>


class VolumeManagerPrivate;

class VolumeManager : public QObject
{
    Q_OBJECT
public:
    static VolumeManager* instance ();

Q_SIGNALS:
    void updateBlockDeviceMountPoints(QStringList& mountPoints);

    // 还未实现
    void updateBurnDeviceMountPoints(QStringList& mountPoints);
    //void addBlockDevice(QString& devName);
    //void delBlockDevice(QString& devName);
    //void updateBlockDevice(QString& devName, QStringList& mountPoints);

private:
    explicit VolumeManager(QObject *parent = nullptr);

private:
    static VolumeManager*               gInstance;
    VolumeManagerPrivate*               d_ptr;
};

#endif // VOLUMEMANAGER_H
