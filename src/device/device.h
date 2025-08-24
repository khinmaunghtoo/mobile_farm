#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QtQml/qqmlregistration.h>
#include "src/scrcpy/scrcpysession.h"

/*
 * Device
 */
class Device: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serial READ getSerial CONSTANT)
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
public:
    Device(QObject *parent = nullptr, QString serial = "");
    ~Device();

    QString getSerial() const { return m_serial; }
    bool isRunning() const { return m_scrcpySession != nullptr; }

    Q_INVOKABLE void startScrcpySession();
    Q_INVOKABLE void stopScrcpySession();

private:
    // device serial number
    QString m_serial;

    // scrcpy session
    ScrcpySession *m_scrcpySession = nullptr;

private slots:
    void handleScrcpySessionOutput(const QByteArray& data);
    void handleScrcpySessionStopped(int exitCode, QProcess::ExitStatus st);

signals:
    void runningChanged();
    void scrcpyError(const QString& error);
};

#endif // DEVICE_H
