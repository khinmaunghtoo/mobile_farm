#ifndef DEVICESCANNER_H
#define DEVICESCANNER_H

#include <QObject>


/*
 * DeviceScanner class
 * This class is responsible for scanning devices with adb.
*/
class DeviceScanner : public QObject
{
    Q_OBJECT

public:
    explicit DeviceScanner(QObject *parent = nullptr);

private:

public slots:

    // scan device;
    void scan();

signals:

    // scan completed signal
    void scanCompleted();

};

#endif // DEVICESCANNER_H
