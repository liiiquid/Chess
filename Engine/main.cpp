#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qfont.h>
#include <qquickstyle.h>
#include "entry/serverentry.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    // Third, register the singleton type provider with QML by calling this function in an initialization function.
    qmlRegisterSingletonInstance("Qt.Singleton", 0, 0, "Server", ServerEntry::instance());

    app.setFont(QFont("Microsoft Yahei"));

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
