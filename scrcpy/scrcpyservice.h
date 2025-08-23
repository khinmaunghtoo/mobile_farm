#ifndef SCRCPYSERVICE_H
#define SCRCPYSERVICE_H

#include <QObject>
#include "scrcpysession.h"
#include <QMap>

// Manages multiple Scrcpy sessions for different devices.
class ScrcpyService : public QObject {
    Q_OBJECT
public:

    // start a session for device with given params
    void startFor(const ScrcpyParams& p) {
        if (sessions.contains(p.deviceSerial)) return;
        auto s = new ScrcpySession(p, this);
        connect(s, &ScrcpySession::output, this, &ScrcpyService::deviceOutput);
        connect(s, &ScrcpySession::stopped, this, &ScrcpyService::deviceStopped);
        sessions.insert(p.deviceSerial, s);
        s->startServer();
    }

    // stop a session for device with given serial
    void stopFor(const QString& deviceSerial) {
        if (auto s = sessions.take(deviceSerial)) { s->stopServer(); s->deleteLater(); }
    }

    // stop all sessions
    void stopAll() {
        for (auto s : std::as_const(sessions)) { s->stopServer(); s->deleteLater(); }
        sessions.clear();
    }

signals:
    void deviceOutput(const QString& deviceSerial, const QByteArray& data);
    void deviceStopped(const QString& deviceSerial, int exitCode, QProcess::ExitStatus);

private:
    QMap<QString, ScrcpySession*> sessions;
};

#endif // SCRCPYSERVICE_H
