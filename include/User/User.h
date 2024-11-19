#ifndef INCLUDE_USER_USER_H_
#define INCLUDE_USER_USER_H_

#include "CacheManager/CacheManager.h"
#include "News/news.h"
#include "Register/Register.h"
#include "Schedule/ScheduleModel.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStringList>
#include <QUrl>

class User : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool registered READ registered NOTIFY registeredChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QUrl pp_location READ pp_location WRITE setPp_location NOTIFY pp_locationChanged)
    Q_PROPERTY(QString bio READ bio WRITE setBio NOTIFY bioChanged)
    // Q_PROPERTY(int level READ level NOTIFY levelChanged)

    Q_PROPERTY(qint32 studentID READ studentID NOTIFY studentIDChanged)
    Q_PROPERTY(QString program READ program NOTIFY programChanged)
    Q_PROPERTY(QString addmission READ addmission NOTIFY addmissionChanged)
    Q_PROPERTY(int year READ year NOTIFY yearChanged)
    Q_PROPERTY(int semester READ semester NOTIFY semesterChanged)
    Q_PROPERTY(int section READ section NOTIFY sectionChanged)
    Q_PROPERTY(QString department READ department NOTIFY departmentChanged)
    Q_PROPERTY(QString stream READ stream NOTIFY streamChanged)

    Q_PROPERTY(
        QStringList profileInfo READ profileInfo NOTIFY profileInfoChanged)

    Q_PROPERTY(Register *registerDB READ registerDB NOTIFY registerDBChanged)
    Q_PROPERTY(Schedule_Model *Schedule READ Schedule NOTIFY scheduleChanged)
    Q_PROPERTY(CacheManager *Cache READ Cache NOTIFY cacheChanged)
    Q_PROPERTY(CacheManager *RegisterCache READ registerCache NOTIFY
                   registerCacheChanged)
    Q_PROPERTY(CacheManager *UserCache READ userCache NOTIFY userCacheChanged)
    Q_PROPERTY(NewsModel *News READ News NOTIFY newsChanged)
  public:
    explicit User(QObject *parent = nullptr);

    Register *registerDB();
    Schedule_Model *Schedule();

    void setProfileInfo();
    void setCache(CacheManager *);
    void setRegisterCache(CacheManager *);
    void setUserCache(CacheManager *);
    Q_INVOKABLE void setHost(const QString &);

    Q_INVOKABLE void openFile(const QUrl &) const;

    void setUserName(const QString &);
    void setPp_location(const QUrl &);
    void setBio(const QString &);

    bool registered() const;
    QString userName() const;
    QUrl pp_location() const;
    QString bio() const;

    // int level() const {
    //     return m_sqlTableAccount.record(0).value("level").toInt();
    // }

    qint32 studentID() const;

    QString program() const;
    QString addmission() const;
    int year() const;
    int semester() const;
    int section() const;
    QString department() const;
    int departmentID() const;
    QString stream() const;
    int streamID() const;
    CacheManager *Cache() const;
    CacheManager *registerCache() const;
    CacheManager *userCache() const;
    NewsModel *News();

    Q_INVOKABLE void registerNew(QString userName, QString pp_location,
                                 QString Bio, qint32 studentID, QString program,
                                 QString addmission, int year, int semester,
                                 int section, QString department,
                                 int departmentIndex, QString stream,
                                 int streamIndex);

    QStringList profileInfo();

  private:
    bool prepareUserDB();
    void prepareScheduleDB();
    QString subHost() const;

  signals:
    void newsChanged();
    void cacheChanged();
    void registerCacheChanged();
    void userCacheChanged();
    void registerDBChanged();
    void scheduleChanged();
    void registeredSuccessfully();
    void profileInfoChanged();
    void registeredChanged();
    void userNameChanged();
    void pp_locationChanged();
    void bioChanged();
    // void levelChanged();

    void studentIDChanged();
    void programChanged();
    void addmissionChanged();
    void yearChanged();
    void semesterChanged();
    void sectionChanged();
    void departmentChanged();
    void streamChanged();

  private:
    QString m_host;
    QString m_userCachePath;

    QSqlDatabase m_db;
    QSqlTableModel m_sqlTableAccount;
    QSqlTableModel m_sqlTableStudent;
    Register *m_register;
    Schedule_Model m_schedule;
    CacheManager *m_mainCache;
    CacheManager *m_registerCache;
    CacheManager *m_userCache;
    bool m_UserDBPrepared;
    NewsModel m_news;
};

#endif // INCLUDE_USER_USER_H_
