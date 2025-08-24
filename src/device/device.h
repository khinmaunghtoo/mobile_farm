#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QtQml/qqmlregistration.h>

/*
 * Device
 */
class Device: public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    Device(QObject *parent = nullptr);
};

#endif // DEVICE_H
