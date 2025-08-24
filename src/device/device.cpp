#include "device.h"
#include <QDebug>

Device::Device(QObject *parent, QString serial) : QObject(parent) {
    if (serial.isEmpty()) {
        qFatal() << "Device serial is empty";
    }
    this->m_serial = serial;
}
