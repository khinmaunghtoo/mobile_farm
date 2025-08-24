#include "devicemanager.h"

DeviceManager::DeviceManager(QObject *parent) : QObject(parent) {}


void DeviceManager::addDevices(QList<Device*> devices) {
    this->m_devices = devices;
    emit devicesChanged();
}
