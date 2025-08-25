#include "scrcpysession.h"
#include <src/config/appconfig.h>
#include <QProcess>
#include "adbrunner.h"

ScrcpySession::ScrcpySession(const ScrcpyParams &params, QObject *parent)
    : QObject(parent), m_scrcpyParams(params), m_process(new QProcess(this))
{

    m_process->setProcessChannelMode(QProcess::MergedChannels);

    // signals: read output from scrcpy process
    connect(m_process, &QProcess::readyRead, this, [this]
            { emit output(m_process->readAll()); });

    // signals: process termination
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int code, QProcess::ExitStatus st)
            { emit stopped(code, st); });
}

ScrcpySession::~ScrcpySession()
{
    stop();
}

void ScrcpySession::start()
{

    // push scrcpy server to device
    if (!ADBRunner::push(
            m_scrcpyParams.deviceSerial,
            AppConfig::scrcpyServerPath(),
            m_scrcpyParams.serverRemotePath))
    {
        emit stopped(-1, QProcess::CrashExit);
        return;
    }

    // setup port forwarding for video stream
    // Try reverse tunnel first (scrcpy 3.x default), fallback to forward if needed
    if (!ADBRunner::reverse(m_scrcpyParams.deviceSerial, 
                           m_scrcpyParams.forwardSocketName, 
                           m_scrcpyParams.tcpPort))
    {
        emit stopped(-1, QProcess::CrashExit);
        return;
    }

    // here we dont use ADBRunner, because scrcpy server is not a simple command
    // but a long-running process that streams video data continuously.
    // ADBRunner is designed for one-shot commands that return and exit,
    // while scrcpy server needs persistent connection and async data handling.
    QStringList args;
    args << "-s" << m_scrcpyParams.deviceSerial
         << "shell"
         << QString("CLASSPATH=%1").arg(m_scrcpyParams.serverRemotePath)
         << "app_process" << "/" << "com.genymobile.scrcpy.Server";

    // server version
    args << m_scrcpyParams.serverVersion;

    // bit rate
    args << QString("video_bit_rate=%1").arg(m_scrcpyParams.bitRate);

    // scrcpy server log level 
    args << QString("log_level=%1").arg(m_scrcpyParams.logLevel);

    // max size
    if (m_scrcpyParams.maxSize > 0)
        args << QString("max_size=%1").arg(m_scrcpyParams.maxSize);

    // max fps
    if (m_scrcpyParams.maxFps > 0)
        args << QString("max_fps=%1").arg(m_scrcpyParams.maxFps);

    // control
    args << QString("control=%1").arg(m_scrcpyParams.control);

    // stay awake
    args << QString("stay_awake=%1").arg(m_scrcpyParams.stayAwake);

    // scid - must match the socket name used in forward
    args << QString("scid=%1").arg(m_scrcpyParams.scid, 8, 16, QChar('0'));

    // TODO: adb path
    m_process->start(AppConfig::adbPath(), args);
}

// stop scrcpy server process
void ScrcpySession::stop()
{
    if (m_process->state() != QProcess::NotRunning)
        m_process->kill();
    m_process->waitForFinished(2000);
}
