#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationDisplayName("Woldia University");

    QGuiApplication::setApplicationName("WLDU");
    QGuiApplication::setOrganizationName("DevaBDM");
    QGuiApplication::setApplicationVersion("0.0.1");
    QGuiApplication::setOrganizationDomain("org");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.loadFromModule("WLDU", "Main");

    return app.exec();
}
