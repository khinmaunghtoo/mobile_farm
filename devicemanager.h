#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "device.h"
#include <QList>

/*
 * DeviceManager
 * responsible for device management
 * - connection to devices
 * - manage device list
 * 
 */
class DeviceManager
{
private:
    // devices
    QList<Device*> devices;
public:
    DeviceManager();
};

#endif // DEVICEMANAGER_H
