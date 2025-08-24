#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "src/device/devicemanager.h"
#include "src/device/devicescanner.h"
#include "src/device/device.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Fusion");


    QQmlApplicationEngine engine;

    // 注册QML类型
    qmlRegisterType<DeviceScanner>("MobileFarm", 1, 0, "DeviceScanner");
    qmlRegisterType<DeviceManager>("MobileFarm", 1, 0, "DeviceManager");
    qmlRegisterType<Device>("MobileFarm", 1, 0, "Device");

    const QUrl url(QStringLiteral("qrc:/MobileFarm/src/qml/Main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
