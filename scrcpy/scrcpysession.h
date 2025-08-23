#ifndef SCRCPYSESSION_H
#define SCRCPYSESSION_H


#include <QObject>
#include <QProcess>

// Scrcpy Server Params
struct ScrcpyParams {
    QString deviceSerial;
    QString serverRemotePath;   // /data/local/tmp/scrcpy-server
    QString serverVersion;      // 例如 "2.6"（与PC端匹配）
    int     bitRate = 8'000'000;
    int     maxSize = 0;        // 0=不限制
    int     maxFps  = 0;        // 0=不限制
    QString logLevel;           // "", "verbose", "debug", "info", "warn", "error"
    bool    control = true;
    QString crop;               // "WxH:x:y" or ""
    bool    tunnelForward = false;
    bool    stayAwake = false;
    QString codecOptions;       // 可空
    QString encoderName;        // 可空
    int     scid = -1;          // -1 让server默认
};

/*
 * Single Scrcpy Session !!! We have (ScrcpyService) to manage multiple sessions.
 * Manages a scrcpy server session for (a specific device) identified by its serial number.
 * Handles starting and stopping the scrcpy server process and emits signals for output and process termination.
 */
class ScrcpySession : public QObject {
    Q_OBJECT
public:
    explicit ScrcpySession(const ScrcpyParams& p, QObject* parent=nullptr)
        : QObject(parent), m(p), proc(new QProcess(this)) {
        proc->setProcessChannelMode(QProcess::MergedChannels);
        connect(proc, &QProcess::readyRead, this, [this]{
            emit output(m.deviceSerial, proc->readAll());
        });
        connect(proc, QOverload<int,QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int code, QProcess::ExitStatus st){
                    emit stopped(m.deviceSerial, code, st);
                });
    }

    void startServer() {
        if (proc->state() != QProcess::NotRunning) stopServer();

        QStringList args;
        args << "-s" << m.deviceSerial
             << "shell"
             << QString("CLASSPATH=%1").arg(m.serverRemotePath)
             << "app_process" << "/" << "com.genymobile.scrcpy.Server";

        // —— 必要：版本（新 server 常用第一个位置参数是版本或配置起始位）——
        if (!m.serverVersion.isEmpty()) args << m.serverVersion;

        // —— 只加“用得上”的关键参数，避免过长 ——
        args << QString("video_bit_rate=%1").arg(m.bitRate);
        if (!m.logLevel.isEmpty())
            args << QString("log_level=%1").arg(m.logLevel);
        if (m.maxSize > 0)
            args << QString("max_size=%1").arg(m.maxSize);
        if (m.maxFps > 0)
            args << QString("max_fps=%1").arg(m.maxFps);
        if (!m.crop.isEmpty())
            args << QString("crop=%1").arg(m.crop);
        if (!m.control)
            args << "control=false";
        if (m.tunnelForward)
            args << "tunnel_forward=true";
        if (m.stayAwake)
            args << "stay_awake=true";
        if (!m.codecOptions.isEmpty())
            args << QString("codec_options=%1").arg(m.codecOptions);
        if (!m.encoderName.isEmpty())
            args << QString("encoder_name=%1").arg(m.encoderName);
        if (m.scid != -1)
            args << QString("scid=%1").arg(m.scid, 8, 16, QChar('0'));

        proc->start("adb", args);
    }

    void stopServer() {
        if (proc->state() != QProcess::NotRunning) proc->kill();
        proc->waitForFinished(2000);
    }

signals:
    void output(const QString& deviceSerial, const QByteArray& data);
    void stopped(const QString& deviceSerial, int exitCode, QProcess::ExitStatus st);

private:
    ScrcpyParams m;
    QProcess* proc;
};

#endif // SCRCPYSESSION_H
