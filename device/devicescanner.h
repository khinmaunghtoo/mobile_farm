#ifndef DEVICESCANNER_H
#define DEVICESCANNER_H

#include <QObject>
#include <qqml.h>
#include <QThread>
#include <QTimer>
#include "adbrunner.h"
#include <QTConcurrent/QtConcurrent>


/*
 * DeviceScanner class
 * This class is responsible for scanning devices with adb.
*/
class DeviceScanner : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool isScanning READ isScanning WRITE setIsScanning NOTIFY isScanningChanged)
    Q_PROPERTY(QStringList devices READ devices WRITE setDevices NOTIFY devicesChanged)



public:
    explicit DeviceScanner(QObject *parent = nullptr);

    // is scanning getter & setter
    bool isScanning() const { return m_isScanning; }
    void setIsScanning(bool scanning) {
        if (m_isScanning == scanning)
            return;
        m_isScanning = scanning;
        emit isScanningChanged();
    }

    // devices getter & setter
    QStringList devices() const { return m_devices; }
    void setDevices(const QStringList &devices) {
        if (m_devices == devices && !devices.isEmpty())
            return;
        m_devices = devices;
        qDebug() << "Devices updated:" << m_devices;
        emit devicesChanged();
    }


private:
    bool m_isScanning = false;
    QStringList m_devices;

public slots:

    // scan device;
    void scan();

signals:
    void isScanningChanged();
    void devicesChanged();
    void error(const QString &message);

};

#endif // DEVICESCANNER_H
