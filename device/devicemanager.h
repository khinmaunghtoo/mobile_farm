#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include "device.h"

/*
 * DeviceManager
 * responsible for device management
 * - connection to devices
 * - manage device list
 * 
 */
class DeviceManager
{
    Q_OBJECT
    QML_ELEMENT
private:
    // devices
    QList<Device*> devices;
public:
    DeviceManager();
};

#endif // DEVICEMANAGER_H
