#include "device.h"
#include <QDebug>

Device::Device(QObject *parent, QString serial) : QObject(parent) {
    if (serial.isEmpty()) {
        qFatal() << "Device serial is empty";
    }
    this->m_serial = serial;
}

Device::~Device() {
    if (m_scrcpySession) {
        delete m_scrcpySession;
        m_scrcpySession = nullptr;
    }
}
    
// start scrcpy session
void Device::startScrcpySession() {
    if (m_scrcpySession) {
        qDebug() << "Scrcpy already running for device:" << m_serial;
        return;
    }

    // scrcpy params
    ScrcpyParams params;
    params.deviceSerial = m_serial;

    // create scrcpy session and connect signals
    m_scrcpySession = new ScrcpySession(params, this);
    connect(m_scrcpySession, &ScrcpySession::output, this, &Device::handleScrcpySessionOutput);
    connect(m_scrcpySession, &ScrcpySession::stopped, this, &Device::handleScrcpySessionStopped);

    // start scrcpy session
    m_scrcpySession->startServer();
    emit runningChanged();

    qDebug() << "Scrcpy started for device:" << m_serial;
}
    
// stop scrcpy session
void Device::stopScrcpySession() {
    if (!m_scrcpySession) {
        return;
    }
    m_scrcpySession->stopServer();
    delete m_scrcpySession;
    m_scrcpySession = nullptr;
    emit runningChanged();

    qDebug() << "Scrcpy stopped for device:" << m_serial;
}
