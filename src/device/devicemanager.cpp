#include "devicemanager.h"

DeviceManager::DeviceManager(QObject *parent) : QObject(parent) {}


void DeviceManager::addDevices(QStringList deviceSerials) {
    qDebug() << "addDevices" << deviceSerials;;
    // this->m_devices = devices;
    // emit devicesChanged();
}
