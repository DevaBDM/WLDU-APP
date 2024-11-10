#include "User/User.h"
#include "CacheManager/CacheManager.h"
#include "Register/Register.h"
#include "Schedule/ScheduleModel.h"
#include "constant.h"
#include <QDateTime>
#include <QDir>

User::User(QObject *parent)
    : QObject{parent}, m_db{QSqlDatabase::addDatabase("QSQLITE", "User")},
      m_sqlTableAccount{parent, m_db}, m_sqlTableStudent{parent, m_db},
      m_register(nullptr), m_schedule{}, m_mainCache{nullptr},
      m_registerCache(nullptr), m_userCache{nullptr}, m_news(this),
      m_downloadManger{this} {
    QDir{}.mkpath("Register");

    if (!prepareUserDB() || !registered()) {
        setUserCache(new CacheManager{"/Register/user", this});
        setRegisterCache(new CacheManager{"/Register/register", this});

        connect(userCache(), &CacheManager::done, [&](CacheManager *cache) {
            prepareUserDB();
            setUserCache(nullptr);
        });

        connect(registerCache(), &CacheManager::done, [&](CacheManager *) {
            m_register = new Register{this};
            connect(this, &User::registeredSuccessfully, m_register,
                    &Register::deleteLater);
            setRegisterCache(nullptr);
            emit registerDBChanged();
        });
    } else if (registered()) {
        prepareScheduleDB();
    }

    connect(this, &User::registeredSuccessfully, &User::prepareScheduleDB);
}

bool User::prepareUserDB() {
    if (m_db.isOpen())
        m_db.close();
    m_db.setDatabaseName("Register/user.db");
    m_db.open();
    m_sqlTableAccount.setTable("Account");
    m_sqlTableStudent.setTable("Student");
    m_UserDBPrepared = m_sqlTableAccount.select() && m_sqlTableStudent.select();
    return m_UserDBPrepared;
}

void User::prepareScheduleDB() {
    QDir{}.mkpath(location());
    m_schedule.setPath(location());
    emit scheduleChanged();
}

bool User::registered() const { return m_sqlTableAccount.rowCount() > 0; }

QString User::userName() const {
    return m_sqlTableAccount.record(0).value("user_name").toString();
}

QUrl User::pp_location() const {
    return m_sqlTableAccount.record(0).value("pp_location").toString();
}

QString User::bio() const {
    return m_sqlTableAccount.record(0).value("Bio").toString();
}
// int level() const {
//     return m_sqlTableAccount.record(0).value("level").toInt();
// }

qint32 User::studentID() const {
    return m_sqlTableStudent.record(0).value("studentID").toInt();
}

QString User::program() const {
    return m_sqlTableStudent.record(0).value("Program").toString();
}

QString User::addmission() const {
    return m_sqlTableStudent.record(0).value("Addmission").toString();
}

int User::year() const {
    return m_sqlTableStudent.record(0).value("year").toInt();
}

int User::semester() const {
    return m_sqlTableStudent.record(0).value("semester").toInt();
}

int User::section() const {
    return m_sqlTableStudent.record(0).value("section").toInt();
}

QString User::department() const {
    return m_sqlTableStudent.record(0).value("Department").toString();
}

int User::departmentID() const {
    return m_sqlTableStudent.record(0).value("pk_Department").toInt();
}

QString User::stream() const {
    return m_sqlTableStudent.record(0).value("Stream").toString();
}

int User::streamID() const {
    return m_sqlTableStudent.record(0).value("pk_Stream").toInt();
}

void User::registerNew(QString userName, QString pp_location, QString Bio,
                       qint32 studentID, QString program, QString addmission,
                       int year, int semester, int section, QString department,
                       int departmentIndex, QString stream, int streamIndex) {
    if (m_UserDBPrepared) {
        QSqlRecord r{m_sqlTableAccount.record()};
        r.setValue("user_name", userName);
        r.setValue("pp_location", pp_location);
        r.setValue("Bio", Bio);
        r.setValue("update_date", QDateTime::currentDateTimeUtc().toString(
                                      "yyyy-MM-dd hh:mm:ss"));
        m_sqlTableAccount.insertRecord(-1, r);
        m_sqlTableAccount.select();

        QSqlRecord rr{m_sqlTableStudent.record()};
        rr.setValue("studentID", studentID);
        rr.setValue("pk_Account", m_sqlTableAccount.rowCount());
        rr.setValue("Program", program);
        rr.setValue("addmission", addmission);
        rr.setValue("year", year);
        rr.setValue("semester", semester);
        rr.setValue("section", section);
        rr.setValue("pk_Department", departmentIndex);
        rr.setValue("Department", department);
        rr.setValue("pk_Stream", streamIndex);
        rr.setValue("Stream", stream);
        rr.setValue("update_date", QDateTime::currentDateTimeUtc().toString(
                                       "yyyy-MM-dd hh:mm:ss"));
        m_sqlTableStudent.insertRecord(-1, rr);
        m_sqlTableStudent.select();

        emit userNameChanged();
        emit pp_locationChanged();
        emit bioChanged();
        emit studentIDChanged();
        emit programChanged();
        emit addmissionChanged();
        emit yearChanged();
        emit semesterChanged();
        emit sectionChanged();
        emit departmentChanged();
        emit streamChanged();

        emit registeredChanged();
        emit registeredSuccessfully();
        emit profileInfoChanged();
    } else if (userCache()) {
        userCache()->reFetch();
    }
}

QStringList User::profileInfo() {
    QStringList model;
    model.append(bio().isEmpty() ? "No Bio" : bio());
    model.append("WDU" + QString::number(studentID()));
    model.append(program() + "(" + addmission() + ")");
    model.append(QString::number(year()) + " Year " +
                 QString::number(semester()) + " semester" + ", section " +
                 QString::number(section()));
    model.append(department() + "(" + stream() + ")");

    return model;
}

QString User::location() const {
    return "Material/" + program() + "/" + addmission() + "/" + "D" +
           QString::number(departmentID()) + "S" + QString::number(streamID()) +
           "Y" + QString::number(year()) + "S" + QString::number(semester()) +
           "s" + QString::number(section());
}

Register *User::registerDB() { return m_register; }
Schedule_Model *User::Schedule() { return &m_schedule; }

CacheManager *User::Cache() const { return m_mainCache; }
CacheManager *User::registerCache() const { return m_registerCache; }
CacheManager *User::userCache() const { return m_userCache; }

void User::setCache(CacheManager *cache) {
    if (!cache && !m_mainCache || cache == m_mainCache)
        return;

    if (m_mainCache)
        m_mainCache->deleteLater();

    m_mainCache = cache;
    emit cacheChanged();
}

void User::setRegisterCache(CacheManager *cache) {
    if (!cache && !m_registerCache || cache == m_registerCache)
        return;

    if (m_registerCache)
        m_registerCache->deleteLater();

    m_registerCache = cache;
    emit registerCacheChanged();
}

void User::setUserCache(CacheManager *cache) {
    if (!cache && !m_userCache || cache == m_userCache)
        return;

    if (m_userCache)
        m_userCache->deleteLater();

    m_userCache = cache;
    emit userCacheChanged();
}

NewsModel *User::News() { return &m_news; }

DownloadManger *User::Download() { return &m_downloadManger; };

void User::downloadFile(QString saveName, QString hash) {
    m_downloadManger.download(Constant::k_hostname + "/" + location(), saveName,
                              hash);
}
