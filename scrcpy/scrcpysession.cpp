#include "scrcpysession.h"




ScrcpySession::ScrcpySession(const QString &serial, QObject *parent)
    : QObject(parent), m_serial(serial), m_process(new QProcess(this))
{
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, &QProcess::readyRead, this, [this] {
        emit serverOutput(m_serial, m_process->readAll());
    });
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int code, QProcess::ExitStatus status){
                emit serverStopped(m_serial, code);
            });
}

void ScrcpySession::startServer() {
    QStringList args;
    args << "-s" << m_serial
         << "shell"
         << "CLASSPATH=/data/local/tmp/scrcpy-server"
         << "app_process" << "/" << "com.genymobile.scrcpy.Server"
         << "0" << "8000000" << "false";
    m_process->start("adb", args);
}

void ScrcpySession::stopServer() {
    if (m_process->state() == QProcess::Running) {
        m_process->kill();
    }
}
