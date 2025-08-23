#include "adbrunner.h"
#include <QDir>
#include <QCoreApplication>
#include <qprocess.h>


ADBRunner::ADBRunner(QObject* parent) 
{}

QString ADBRunner::adbPath() const {
    QString base = QCoreApplication::applicationDirPath();
    QString path;

#if defined(Q_OS_WIN)
    path = QDir(base).filePath("third_party/adb/win/adb.exe");
#elif defined(Q_OS_MACOS)
    path = QDir(base).filePath("third_party/adb/mac/adb");
#elif defined(Q_OS_LINUX)
    path = QDir(base).filePath("third_party/adb/linux/adb");
#else
    path = "adb";  // fallback，假设系统 PATH 有 adb
#endif

    return path;
}

QString ADBRunner::scrcpyServerPath() const {
    QString base = QCoreApplication::applicationDirPath();
    return QDir(base).filePath("third_party/scrcpy-server");
}


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
        qWarning() << "Local file not found:" << localPath;
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
        qWarning() << "adb push failed:" << output;
        return false;
    }

    return true;
}

void ADBRunner::reverse(const QString &deviceSerial,
                        const QString &deviceSocketName,
                        quint16 localPort)
{
    
    QStringList args;
    args << "reverse";
    args << QString("localabstract:%1").arg(deviceSocketName);
    args << QString("tcp:%1").arg(localPort);
    executeADBCommand(args);
}

void ADBRunner::forward(const QString &deviceSerial,
                        const QString &deviceSocketName,
                        quint16 localPort)
{
    
    QStringList args;
    args << "forward";
    args << QString("localabstract:%1").arg(deviceSocketName);
    args << QString("tcp:%1").arg(localPort);
    executeADBCommand(args);
}




QString ADBRunner::executeADBCommand(const QStringList &args)
{
    QProcess process;
    process.start(adbPath(), args);

    if (!process.waitForStarted(5000))
        return QString("[error] failed to start adb: %1").arg(process.errorString());

    process.waitForFinished(-1);
    QString out = QString::fromUtf8(process.readAllStandardOutput());
    QString err = QString::fromUtf8(process.readAllStandardError());
    if (!err.isEmpty()) out += "\n[stderr]\n" + err;
    return out.trimmed();
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


