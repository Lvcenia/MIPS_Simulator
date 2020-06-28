#ifndef VMDISK_H
#define VMDISK_H

#include <QObject>

class vmDisk : public QObject
{
    Q_OBJECT
public:
    explicit vmDisk(QObject *parent = nullptr);

signals:

};

#endif // VMDISK_H
