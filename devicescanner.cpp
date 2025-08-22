#include "devicescanner.h"

DeviceScanner::DeviceScanner(QObject *parent)
    : QObject(parent) {
}


void DeviceScanner::scan() {
    qDebug("start scanning devices...");
    setIsScanning(true);

    auto devices = ADBRunner::scanDevices();
    setDevices(devices);


    setIsScanning(false);

}
