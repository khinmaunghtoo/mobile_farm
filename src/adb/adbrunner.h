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

    // forward
    static bool forward(const QString &serial, const QString &deviceSocketName, quint16 localPort);
    // remove forward
    static bool removeForward(const QString &serial, quint16 localPort);
    // remove forward by socket name
    static bool removeForwardBySocketName(const QString &serial, const QString &deviceSocketName);
    

    // reverse
    static bool reverse(const QString &serial, const QString &deviceSocketName, quint16 localPort);
    // remove reverse
    static bool removeReverse(const QString &serial, quint16 localPort);
    // remove reverse by socket name
    static bool removeReverseBySocketName(const QString &serial, const QString &deviceSocketName);



private:
    // Execute ADB command
    static QString executeADBCommand(const QStringList& args);

};

#endif // ADBRUNNER_H
