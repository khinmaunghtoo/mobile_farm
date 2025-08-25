#ifndef SCRCPYSESSION_H
#define SCRCPYSESSION_H

#include <QObject>
#include <QProcess>
#include <QRandomGenerator>
#include "scrcpyparams.h"

#include <QTcpServer>
#include <QTcpSocket>

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

    void startScrcpyServer();

    void stopProcess();

signals:
    void output(const QByteArray &data);
    void stopped(int exitCode, QProcess::ExitStatus st);
    void videoData(const QByteArray &data);

private:
    ScrcpyParams m_scrcpyParams;
    QProcess *m_process;

    // used for video stream (device screen -> pc)
    QTcpServer *m_videoSocket;

    // used for control (pc control -> device)
    QTcpSocket *m_controlSocket;
};

#endif // SCRCPYSESSION_H
