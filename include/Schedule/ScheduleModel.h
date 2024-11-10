#ifndef INCLUDE_SRC_SCHEDULEMODEL_H_
#define INCLUDE_SRC_SCHEDULEMODEL_H_

#include "CacheManager/CacheManager.h"
#include "Schedule/FilesModel.h"
#include <QAbstractListModel>
#include <QDate>
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
    Q_PROPERTY(CacheManager *Cache READ Cache NOTIFY CacheChanged)
    Q_PROPERTY(Files_Model *FilesModel READ FilesModel NOTIFY FilesModelChanged)
  public:
    explicit Schedule_Model(QAbstractListModel *parent = nullptr);
    explicit Schedule_Model(QString p, QAbstractListModel *parent = nullptr);
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

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;

    // properties
    int currentRow() const;
    void setCurrentRow(int row);
    void setEpoch(int);
    void setPath(QString p);
    void setCache(CacheManager *);

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
    CacheManager *Cache() const;
    Files_Model *FilesModel();

  private:
    bool prepareModelDB();
    void connectCurrentRow();

  public slots:
    bool setFilter();
    void nextDay();
    void previousDay();
    void currentDate();
    void setWeekDay(int week);
    void fetch();
    int epoch() const;

  signals:
    void FilesModelChanged();
    void CacheChanged();
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
    void epochChanged();

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    int m_currentRow;
    QDate m_date;
    int m_epoch;
    QString m_filter;
    QString m_path;
    CacheManager *m_mainCache;
    Files_Model m_filesModel;
};

#endif // INCLUDE_SRC_SCHEDULEMODEL_H_
