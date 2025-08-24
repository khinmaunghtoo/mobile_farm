#ifndef SCRCPYSESSION_H
#define SCRCPYSESSION_H

#include <QObject>
#include <QProcess>

// Scrcpy Server Params
struct ScrcpyParams
{
    QString deviceSerial;
    QString serverRemotePath = "/data/local/tmp/scrcpy-server"; // /data/local/tmp/scrcpy-server
    QString serverVersion = "2.6";                              // 例如 "2.6"（与PC端匹配）
    int bitRate = 8'000'000;
    int maxSize = 0;           // 0=不限制
    int maxFps = 0;            // 0=不限制
    QString logLevel = "info"; // "", "verbose", "debug", "info", "warn", "error"
    bool control = true;
    QString crop = ""; // "WxH:x:y" or ""
    bool tunnelForward = false;
    bool stayAwake = false;
    QString codecOptions = ""; // 可空
    QString encoderName = "";  // 可空
    int scid = -1;             // -1 让server默认
};

/*
 * Single Scrcpy Session !!! Every device has its own ScrcpySession!!!.
 * - starting and stopping the scrcpy server process
 * - handling (scrcpy server process) output and termination
 */
class ScrcpySession : public QObject
{
    Q_OBJECT
public:
    explicit ScrcpySession(const ScrcpyParams &params, QObject *parent = nullptr);
    ~ScrcpySession();

    void start();

    void stop();

signals:
    void output(const QByteArray &data);
    void stopped(int exitCode, QProcess::ExitStatus st);

private:
    ScrcpyParams m_params;
    QProcess *m_process;
};

#endif // SCRCPYSESSION_H
