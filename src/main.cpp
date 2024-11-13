#include "Update/AppUpdate.h"
#include "User/User.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationDisplayName("Woldia University");

    QGuiApplication::setApplicationName("WLDU");
    QGuiApplication::setOrganizationName("DevaBDM");
    QGuiApplication::setApplicationVersion("0.0.1");
    QGuiApplication::setOrganizationDomain("org");

    AppUpdate update;
    User user;

    qmlRegisterSingletonInstance("com.user.db", 1, 0, "User", &user);
    qmlRegisterSingletonInstance("com.AppUpdate", 1, 0, "AppUpdate", &update);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.loadFromModule("WLDU", "Main");

    return app.exec();
}
