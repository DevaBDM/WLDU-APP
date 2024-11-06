#include "Register/Register.h"
#include "Schedule/ScheduleModel.h"
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

    User user;
    Register reg;

    Schedule_Model scheduleModel;

    qmlRegisterSingletonInstance("com.register.db", 1, 0, "Register", &reg);
    qmlRegisterSingletonInstance("com.user.db", 1, 0, "User", &user);
    qmlRegisterSingletonInstance("com.schedule.db", 1, 0, "ScheduleModel",
                                 &scheduleModel);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.loadFromModule("WLDU", "Main");

    return app.exec();
}
