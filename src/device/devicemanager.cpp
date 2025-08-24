#include "devicemanager.h"

DeviceManager::DeviceManager(QObject *parent) : QObject(parent) {}


void DeviceManager::addDevices(QStringList deviceSerials) {
    for (const QString &deviceSerial : deviceSerials) {
        Device *device = new Device(this, deviceSerial);
        this->m_devices.append(device);
    }
    emit devicesChanged();
}
