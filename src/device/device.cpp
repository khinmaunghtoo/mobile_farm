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

    // auto assign a unique port starting from 27183
    if (params.tcpPort == 0) {
        static quint16 nextPort = 27183;
        params.tcpPort = nextPort++;
        qDebug() << "Auto-assigned port" << params.tcpPort << "for device" << params.deviceSerial;
    }

    // update socket name with scid for unique identification
    params.socketName = QString("scrcpy_%1").arg(params.scid, 8, 16, QChar('0'));

    // create scrcpy session and connect signals
    m_scrcpySession = new ScrcpySession(params, this);
    connect(m_scrcpySession, &ScrcpySession::output, this, &Device::handleScrcpySessionOutput);
    connect(m_scrcpySession, &ScrcpySession::stopped, this, &Device::handleScrcpySessionStopped);
    connect(m_scrcpySession, &ScrcpySession::videoData, this, &Device::handleVideoStream);

    // start scrcpy session
    m_scrcpySession->startScrcpyServer();
    emit runningChanged();

}
    
// stop scrcpy session
void Device::stopScrcpySession() {
    if (!m_scrcpySession) {
        return;
    }
    m_scrcpySession->stop();
    delete m_scrcpySession;
    m_scrcpySession = nullptr;
    emit runningChanged();

    qDebug() << "Scrcpy stopped for device:" << m_serial;
}


// handle scrcpy session output
// scrcpy server 连接成功后，会输出一些信息
void Device::handleScrcpySessionOutput(const QByteArray& data) {
    qDebug() << "Scrcpy output for device:" << m_serial << "\n" << data;
}

// handle scrcpy session stopped
void Device::handleScrcpySessionStopped(int exitCode, QProcess::ExitStatus st) {
    Q_UNUSED(exitCode);
    Q_UNUSED(st);
    if (m_scrcpySession) {
        delete m_scrcpySession;
        m_scrcpySession = nullptr;
        emit runningChanged();
    }
}


// handle video stream
void Device::handleVideoStream(const QByteArray& data) {
    qDebug() << "Received video data for device:" << m_serial << "size:" << data.size() << "bytes";
}
