#include "devicescanner.h"

DeviceScanner::DeviceScanner(QObject *parent)
    : QObject(parent) {
}


void DeviceScanner::scan() noexcept(false) {
    qDebug("start scanning devices...");
    setIsScanning(true);

    QThreadPool::globalInstance()->start([this] {
        try {
            auto devices = ADBRunner::scanDevices();
            QMetaObject::invokeMethod(this, [this, devices]{
                setDevices(devices);
                setIsScanning(false);
            }, Qt::QueuedConnection);
        } catch (const std::exception &e) {
            qWarning() << "Device scan error:" << e.what();
            QMetaObject::invokeMethod(this, [this, errorMsg = QString::fromUtf8(e.what())] {
                setIsScanning(false);
                emit error(errorMsg);  // 確保在主線程中發射信號
            }, Qt::QueuedConnection);
        }
    });
}
