#ifndef ADBRUNNER_H
#define ADBRUNNER_H
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QRegularExpression>

static QStringList parseDeviceList(const QString &output);

/*
 * ADBRunner class
 * This class is responsible for run adb commands
 */
class ADBRunner
{
public:
    ADBRunner();

    // adb path
    static QString adbPath();

    // scan devices
    static QStringList scanDevices();

private:

    // excute adb command
    static QString executeADBCommand(const QStringList &args);

};

#endif // ADBRUNNER_H
