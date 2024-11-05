#ifndef INCLUDE_SRC_SCHEDULEMODEL_H_
#define INCLUDE_SRC_SCHEDULEMODEL_H_

#include <QAbstractListModel>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlTableModel>

class Schedule_Model : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int currentRow READ currentRow WRITE setCurrentRow NOTIFY
                   currentRowChanged)
    Q_PROPERTY(QVariant startTime READ startTime NOTIFY startTimeChanged)
    Q_PROPERTY(QVariant endTime READ endTime NOTIFY endTimeChanged)
    Q_PROPERTY(QVariant title READ title NOTIFY titleChanged)
    Q_PROPERTY(QVariant shortNote READ shortNote NOTIFY shortNoteChanged)
    Q_PROPERTY(QVariant teacherName READ teacherName NOTIFY teacherNameChanged)
    Q_PROPERTY(QVariant teacherPP READ teacherPP NOTIFY teacherPPChanged)
    Q_PROPERTY(QVariant description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QVariant type READ type NOTIFY typeChanged)
    Q_PROPERTY(QVariant slipID READ slipID NOTIFY slipIDChanged)
    Q_PROPERTY(QVariant scheduleID READ scheduleID NOTIFY scheduleIDChanged)
    Q_PROPERTY(QVariant status READ status NOTIFY statusChanged)
    Q_PROPERTY(QVariant seen READ seen NOTIFY seenChanged)

    Q_PROPERTY(QVariant date READ date NOTIFY dateChanged)
    Q_PROPERTY(QVariant weekDay READ weekDay NOTIFY dateChanged)
    Q_PROPERTY(NetworkStatus networkStatus READ networkStatus NOTIFY
                   networkStatusChanged)
  public:
    explicit Schedule_Model(QObject *parent = nullptr);
    enum Role {
        startTimeRole = Qt::UserRole + 1,
        endTimeRole,
        titleRole,
        shortNoteRole,
        teacherNameRole,
        teacherPPRole,
        descriptionRole,
        typeRole,
        slipIDRole,
        scheduleIDRole,
        statusRole,
        seenRole
    };
    enum class NetworkStatus {
        Connecting,
        Requesting,
        Redirected,
        Receiving,
        Connected,
        Downloading,
        Waiting,
        Error
    };
    Q_ENUM(NetworkStatus)

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;

    // properties
    int currentRow() const;
    void setCurrentRow(int row);
    void setNetworkStatus(NetworkStatus);
    void setEpoch(int);

    QVariant startTime() const;
    QVariant endTime() const;
    QVariant title() const;
    QVariant shortNote() const;
    QVariant teacherName() const;
    QVariant teacherPP() const;
    QVariant description() const;
    QVariant type() const;
    QVariant slipID() const;
    QVariant scheduleID() const;
    QVariant status() const;
    QVariant seen() const;
    QVariant date() const;
    QVariant weekDay() const;
    NetworkStatus networkStatus() const;

    void downloadScheduleDB();

  public slots:
    void setFilter();
    void nextDay();
    void previousDay();
    void currentDate();
    void setWeekDay(int week);
    void fetch();
    QString networkMessage(NetworkStatus) const;
    int epoch() const;

  signals:
    void currentRowChanged(int);

    void startTimeChanged();
    void endTimeChanged();
    void titleChanged();
    void shortNoteChanged();
    void teacherNameChanged();
    void teacherPPChanged();
    void descriptionChanged();
    void typeChanged();
    void slipIDChanged();
    void scheduleIDChanged();
    void statusChanged();
    void seenChanged();
    void dateChanged();
    void networkStatusChanged();
    void epochChanged();

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    int m_currentRow;
    QDate m_date;
    NetworkStatus m_networkStatus;
    QNetworkAccessManager m_nm;
    int m_epoch;
    QString m_filter;
};

#endif // INCLUDE_SRC_SCHEDULEMODEL_H_
