#ifndef ADBRUNNER_H
#define ADBRUNNER_H

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QRegularExpression>
#include <QMutex>
#include <memory>

class ADBRunner
{
    Q_OBJECT
public:
    // Delete copy constructor and assignment operator
    ADBRunner(const ADBRunner&) = delete;
    ADBRunner& operator=(const ADBRunner&) = delete;

    // Get singleton instance
    static ADBRunner& instance() {
        static std::unique_ptr<ADBRunner> instance;
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!instance) {
            instance.reset(new ADBRunner());
        }
        return *instance.get();
    }

    // Get ADB path
    QString adbPath() const;

    // Get scrcpy-server file path
    QString scrcpyServerPath() const;

    // Scan for connected devices
    QStringList scanDevices();

    // Push file to device
    bool push(const QString& serial, const QString& localPath, const QString& remotePath);

    // reverse
    void reverse(const QString &serial, const QString &deviceSocketName, quint16 localPort);

    // forward
    void forward(const QString &serial, const QString &deviceSocketName, quint16 localPort);

    // start scrcpy
    void startScrcpyServer(const QString &serial);


private:
    explicit ADBRunner(QObject* parent = nullptr);
    ~ADBRunner() = default;

    // Execute ADB command
    QString executeADBCommand(const QStringList& args);

    // Parse device list from ADB output
    static QStringList parseDeviceList(const QString& output);

    QMutex m_mutex;  // For thread safety
};

#endif // ADBRUNNER_H