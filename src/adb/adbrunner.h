#ifndef ADBRUNNER_H
#define ADBRUNNER_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QRegularExpression>


/*
 * ADBRunner responsible for running ADB commands.
*/
class ADBRunner final
{
public:
    ADBRunner() = delete;
    ~ADBRunner() = delete;

    // Scan for connected devices
    static QStringList scanDevices();

    // Push file to device
    static bool push(const QString& serial, const QString& localPath, const QString& remotePath);

    // reverse
    static void reverse(const QString &serial, const QString &deviceSocketName, quint16 localPort);

    // forward
    static void forward(const QString &serial, const QString &deviceSocketName, quint16 localPort);

    // start scrcpy
    static void startScrcpyServer(const QString &serial);


private:
    // Execute ADB command
    static QString executeADBCommand(const QStringList& args);

};

#endif // ADBRUNNER_H
