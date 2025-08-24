#ifndef SCRCPYSESSION_H
#define SCRCPYSESSION_H


#include <QObject>
#include <QProcess>

// Scrcpy Server Params
struct ScrcpyParams {
    QString deviceSerial;
    QString serverRemotePath = "/data/local/tmp/scrcpy-server";   // /data/local/tmp/scrcpy-server
    QString serverVersion = "2.6";      // 例如 "2.6"（与PC端匹配）
    int     bitRate = 8'000'000;
    int     maxSize = 0;        // 0=不限制
    int     maxFps  = 0;        // 0=不限制
    QString logLevel = "info";           // "", "verbose", "debug", "info", "warn", "error"
    bool    control = true;
    QString crop = "";               // "WxH:x:y" or ""
    bool    tunnelForward = false;
    bool    stayAwake = false;
    QString codecOptions = "";       // 可空
    QString encoderName = "";        // 可空
    int     scid = -1;          // -1 让server默认
};

/*
 * Single Scrcpy Session !!! Every device has its own ScrcpySession!!!.
 * - starting and stopping the scrcpy server process 
 * - handling (scrcpy server process) output and termination
 */
class ScrcpySession : public QObject {
    Q_OBJECT
public:
    explicit ScrcpySession(const ScrcpyParams& params, QObject* parent=nullptr)
        : QObject(parent), m_params(params), m_process(new QProcess(this)) {
        m_process->setProcessChannelMode(QProcess::MergedChannels);

        // signals: read output from scrcpy process
        connect(m_process, &QProcess::readyRead, this, [this]{
            emit output(m_process->readAll());
        });

        // signals: process termination
        connect(m_process, QOverload<int,QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int code, QProcess::ExitStatus st){
                    emit stopped(code, st);
                });
    }

    void startServer() {
        if (m_process->state() != QProcess::NotRunning) stopServer();

        QStringList args;
        args << "-s" << m_params.deviceSerial
             << "shell"
             << QString("CLASSPATH=%1").arg(m_params.serverRemotePath)
             << "app_process" << "/" << "com.genymobile.scrcpy.Server";

        // —— 必要：版本（新 server 常用第一个位置参数是版本或配置起始位）——
        if (!m_params.serverVersion.isEmpty()) args << m_params.serverVersion;

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
        m_process->start("adb", args);
    }

    void stopServer() {
        if (m_process->state() != QProcess::NotRunning) m_process->kill();
        m_process->waitForFinished(2000);
    }

signals:
    void output(const QByteArray& data);
    void stopped(int exitCode, QProcess::ExitStatus st);

private:
    ScrcpyParams m_params;
    QProcess* m_process;
};

#endif // SCRCPYSESSION_H
