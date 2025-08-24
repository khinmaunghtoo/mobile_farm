#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "device.h"
#include <QList>
#include <QObject>
#include <QtQml/qqmlregistration.h>

/*
 * DeviceManager
 * responsible for device management
 * - connection to devices
 * - manage device list
 * 
 */
class DeviceManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QList<Device*> devices READ getDevices NOTIFY devicesChanged)
private:
    // devices
    QList<Device*> m_devices;
public:
    DeviceManager(QObject *parent = nullptr);
    QList<Device*> getDevices() const { return m_devices; }

signals:
    void devicesChanged();

public slots:
    void addDevices(QList<Device*> devices);
};

#endif // DEVICEMANAGER_H
