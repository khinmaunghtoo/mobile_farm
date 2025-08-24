#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Fusion"); // 或 "Material"、"Fusion" 等


    QQmlApplicationEngine engine;

    // 注意：路径应该是 qrc:/MobileFarm/src/qml/Main.qml
    const QUrl url(QStringLiteral("qrc:/MobileFarm/src/qml/Main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
