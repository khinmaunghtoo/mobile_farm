#include "adbrunner.h"
#include <QDir>
#include <QCoreApplication>



ADBRunner::ADBRunner() {}

QString ADBRunner::adbPath() {
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


QStringList ADBRunner::scanDevices() {
    QString output = executeADBCommand({"devices"});
    return parseDeviceList(output);
}


QString ADBRunner::executeADBCommand(const QStringList &args)
{
    QProcess p;
    p.start(adbPath(), args);

    if (!p.waitForStarted(5000))
        return QString("[error] failed to start adb: %1").arg(p.errorString());

    p.waitForFinished(-1);
    QString out = QString::fromUtf8(p.readAllStandardOutput());
    QString err = QString::fromUtf8(p.readAllStandardError());
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
