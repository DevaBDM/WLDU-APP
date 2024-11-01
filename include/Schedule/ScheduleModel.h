#ifndef INCLUDE_SRC_SCHEDULEMODEL_H_
#define INCLUDE_SRC_SCHEDULEMODEL_H_

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
  public:
    explicit Schedule_Model(QObject *parent = nullptr);
    enum Role {
        startTimeRole,
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

  public slots:
    void setFilter();
    void nextDay();
    void previousDay();
    void currentDate();

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

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    int m_currentRow;
    QDate m_date;
};

#endif // INCLUDE_SRC_SCHEDULEMODEL_H_
