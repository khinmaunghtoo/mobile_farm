#include "adbrunner.h"
#include <QDir>
#include <QCoreApplication>
#include <qprocess.h>
#include <src/config/appconfig.h>


QStringList parseDeviceList(const QString &output);



QStringList ADBRunner::scanDevices() noexcept(false)
{
    QString output = executeADBCommand({"devices"});

    // check if output contains error, if so, throw exceptio
    if (output.contains("error")) {
        throw std::runtime_error(output.toStdString());
    }

    return parseDeviceList(output);
}


bool ADBRunner::push(const QString &deviceSerial,
                     const QString &localPath,
                     const QString &remotePath)
{
    // check if file exists
    if (!QFileInfo::exists(localPath)) {
        qDebug() << "Local file not found:" << localPath;
        return false;
    }

    // create adb cmd: adb -s <serial> push <localPath> <remotePath>
    QStringList args;
    args << "-s" << deviceSerial
         << "push"
         << localPath
         << remotePath;

    QString output = executeADBCommand(args);

    // check error
    if (output.contains("error")) {
        qDebug() << "adb push failed:" << output;
        return false;
    }

    return true;
}

bool ADBRunner::reverse(const QString &deviceSerial,
                        const QString &deviceSocketName,
                        quint16 localPort)
{
    
    QStringList args;
    args << "reverse";
    args << QString("localabstract:%1").arg(deviceSocketName);
    args << QString("tcp:%1").arg(localPort);
    QString output = executeADBCommand(args);

    // check error
    if (output.contains("error")) {
        qDebug() << "adb reverse failed:" << output;
        return false;
    }

    return true;
}

bool ADBRunner::removeReverse(const QString &deviceSerial,
                              quint16 localPort)
{
    QStringList args;
    args << "reverse";
    args << "--remove";
    args << QString("tcp:%1").arg(localPort);
    QString output = executeADBCommand(args);

    // check error
    if (output.contains("error")) {
        qDebug() << "adb removeReverse failed:" << output;
        return false;
    }

    return true;
}

bool ADBRunner::removeReverseBySocketName(const QString &deviceSerial,
                                          const QString &deviceSocketName)
{
    
    QStringList args;
    args << "reverse";
    args << "--remove";
    args << QString("localabstract:%1").arg(deviceSocketName);
    QString output = executeADBCommand(args);

    // check error
    if (output.contains("error")) {
        qDebug() << "adb removeReverseBySocketName failed:" << output;
        return false;
    }

    return true;
}

bool ADBRunner::forward(const QString &deviceSerial,
                        const QString &deviceSocketName,
                        quint16 localTCPPort)
{
    
    QStringList args;
    args << "-s" << deviceSerial;
    args << "forward";
    args << QString("tcp:%1").arg(localTCPPort);               // local: PC端口
    args << QString("localabstract:%1").arg(deviceSocketName); // remote: 设备socket
    QString output = executeADBCommand(args);

    // check error
    if (output.contains("error")) {
        qDebug() << "adb forward failed:" << output;
        return false;
    }

    return true;
}

bool ADBRunner::removeForward(const QString &deviceSerial,
                              quint16 tcpPort)
{
    
    QStringList args;
    args << "-s" << deviceSerial;
    args << "forward";
    args << "--remove";
    args << QString("tcp:%1").arg(tcpPort);
    QString output = executeADBCommand(args);

    // check error
    if (output.contains("error")) {
        qDebug() << "adb removeforward failed:" << output;
        return false;
    }

    return true;
}   

bool ADBRunner::removeForwardBySocketName(const QString &deviceSerial,
                                          const QString &deviceSocketName)
{
    
    QStringList args;
    args << "-s" << deviceSerial;
    args << "forward";
    args << "--remove";
    args << QString("localabstract:%1").arg(deviceSocketName);
    QString output = executeADBCommand(args);

    // check error
    if (output.contains("error")) {
        qDebug() << "adb removeforward failed:" << output;
        return false;
    }

    return true;
}   



QString ADBRunner::executeADBCommand(const QStringList &args)
{
    qDebug() << "Executing ADB command: " << AppConfig::adbPath() << args;

    QProcess process;
    process.start(AppConfig::adbPath(), args);

    if (!process.waitForStarted(5000))
        return QString("[error] failed to start adb: %1").arg(process.errorString());

    process.waitForFinished(-1);
    
    // 检查退出码
    if (process.exitCode() != 0) {
        QString err = QString::fromUtf8(process.readAllStandardError());
        return QString("[error] adb command failed (exit code %1): %2")
               .arg(process.exitCode())
               .arg(err.isEmpty() ? "unknown error" : err);
    }
    
    // 成功时返回标准输出
    return QString::fromUtf8(process.readAllStandardOutput()).trimmed();
}


QStringList parseDeviceList(const QString &output) {
    QStringList serials;

    // use static to avoid creating QRegularExpression object repeatedly
    static const QRegularExpression lineRegex(QStringLiteral(R"(\r?\n)"));

    const QStringList lines = output.split(lineRegex, Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        const QString trimmedLine = line.trimmed();
        if (trimmedLine.isEmpty()) continue;

        const QStringList parts = trimmedLine.split(QLatin1Char('\t'));
        if (parts.size() == 2 && parts[1] == QStringLiteral("device")) {
            serials << parts[0];
        }
    }
    return serials;
}
