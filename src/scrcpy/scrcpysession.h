#ifndef SCRCPYSESSION_H
#define SCRCPYSESSION_H

#include <QObject>
#include <QProcess>
#include <QRandomGenerator>

/*
   Scrcpy Server Params
   some args are optional, we use default values
   so, they are not shown in the struct
    - codecOptions
    - encoderName
    - crop
*/ 
struct ScrcpyParams
{
    QString deviceSerial;
    const QString serverRemotePath = "/data/local/tmp/scrcpy-server.jar"; // /data/local/tmp/scrcpy-server.jar
    const QString serverVersion = "3.3.1"; // current scrcpy-server version 3.3.1
    const QString logLevel = "info"; // "", "verbose", "debug", "info", "warn", "error"
    int bitRate = 2000000; // 2Mbps
    int maxSize = 0;           // 0 = original size
    int maxFps = 0;            // 0 = original fps
    bool control = true;
    bool tunnelReverse = true; // from 3.0, tunnel reverse is the default
    bool stayAwake = true;

    // device socket name for forwarding, example: scrcpy_12345678
    QString forwardSocketName = "";  
    
    // this value will be generated in ScrcpySession constructor
    quint16 tcpPort = 0;        // local TCP port for forwarding

    // generate our own random scid in stead of using the default scid (-1)
    // then we can use it to identify the scrcpy server process
    const int scid = QRandomGenerator::global()->generate() & 0x7FFFFFFF;

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
    ScrcpyParams m_scrcpyParams;
    QProcess *m_process;
};

#endif // SCRCPYSESSION_H
