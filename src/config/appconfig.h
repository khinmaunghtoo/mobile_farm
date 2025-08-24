#include <QString>
#include <QDir>
#include <QCoreApplication>

class AppConfig
{
public:
    AppConfig() = delete;
    ~AppConfig() = delete;

    static QString base() { return QCoreApplication::applicationDirPath(); }

    // Get ADB path
    static QString adbPath()
    {
        QString path;

#if defined(Q_OS_WIN)
        path = QDir(base()).filePath("third_party/adb/win/adb.exe");
#elif defined(Q_OS_MACOS)
        path = QDir(base()).filePath("third_party/adb/mac/adb");
#elif defined(Q_OS_LINUX)
        path = QDir(base()).filePath("third_party/adb/linux/adb");
#else
        path = "adb"; // fallback，假设系统 PATH 有 adb
#endif

        return path;
    }

    // Get scrcpy-server path
    static QString scrcpyServerPath()
    {
        return QDir(base()).filePath("third_party/scrcpy-server");
    }
};
