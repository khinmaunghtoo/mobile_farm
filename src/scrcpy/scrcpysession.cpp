#include "scrcpysession.h"
#include <src/config/appconfig.h>

ScrcpySession::ScrcpySession(const ScrcpyParams &params, QObject *parent)
    : QObject(parent), m_params(params), m_process(new QProcess(this))
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

ScrcpySession::~ScrcpySession() {
    stop();
}

void ScrcpySession::start()
{
    if (m_process->state() != QProcess::NotRunning)
        stop();

    QStringList args;
    args << "-s" << m_params.deviceSerial
         << "shell"
         << QString("CLASSPATH=%1").arg(m_params.serverRemotePath)
         << "app_process" << "/" << "com.genymobile.scrcpy.Server";

    // —— 必要：版本（新 server 常用第一个位置参数是版本或配置起始位）——
    if (!m_params.serverVersion.isEmpty())
        args << m_params.serverVersion;

    // —— 只加“用得上”的关键参数，避免过长 ——
    args << QString("video_bit_rate=%1").arg(m_params.bitRate);
    if (!m_params.logLevel.isEmpty())
        args << QString("log_level=%1").arg(m_params.logLevel);
    if (m_params.maxSize > 0)
        args << QString("max_size=%1").arg(m_params.maxSize);
    if (m_params.maxFps > 0)
        args << QString("max_fps=%1").arg(m_params.maxFps);
    if (!m_params.crop.isEmpty())
        args << QString("crop=%1").arg(m_params.crop);
    if (!m_params.control)
        args << "control=false";
    if (m_params.tunnelForward)
        args << "tunnel_forward=true";
    if (m_params.stayAwake)
        args << "stay_awake=true";
    if (!m_params.codecOptions.isEmpty())
        args << QString("codec_options=%1").arg(m_params.codecOptions);
    if (!m_params.encoderName.isEmpty())
        args << QString("encoder_name=%1").arg(m_params.encoderName);
    if (m_params.scid != -1)
        args << QString("scid=%1").arg(m_params.scid, 8, 16, QChar('0'));

    // TODO: adb path
    m_process->start(AppConfig::adbPath(), args);
}


// stop scrcpy server process
void ScrcpySession::stop() {
    if (m_process->state() != QProcess::NotRunning)
        m_process->kill();
    m_process->waitForFinished(2000);
}
