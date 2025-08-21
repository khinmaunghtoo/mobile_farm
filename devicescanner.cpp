#include "devicescanner.h"

DeviceScanner::DeviceScanner(QObject *parent)
    : QObject(parent) {
}


void DeviceScanner::scan() {
    qDebug("Scanning for devices...");
}

