#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <frameless.h>
#include <board/board.h>
#include <QFont>
int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setFont(QFont("Microsoft Yahei"));

    QQuickStyle::setStyle("Material");

    QLoggingCategory::setFilterRules(QStringLiteral("qt.scenegraph.general=true"));
    qSetMessagePattern("%{category}: %{message}");

    QQmlApplicationEngine engine;
    qmlRegisterType<Frameless>("Qt.Window", 0, 0, "Frameless");
    qmlRegisterType<Board>("Qt.Window", 0, 0, "Board");
    qmlRegisterSingletonType(QUrl("qrc:/controls/Skin.qml"), "Qt.Singleton", 0, 0, "Skin");

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
