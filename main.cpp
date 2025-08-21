#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "devicescanner.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Register types
    qmlRegisterType<DeviceScanner>("MobileFarm",1,0,"DeviceScanner");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("MobileFarm", "Main");


    return app.exec();
}
