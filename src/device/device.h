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
    Q_PROPERTY(QString serial READ getSerial CONSTANT)
public:
    Device(QObject *parent = nullptr, QString serial = "");

    QString getSerial() const { return m_serial; }

private:
    // device serial number
    QString m_serial;

signals:

};

#endif // DEVICE_H
