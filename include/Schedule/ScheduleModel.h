#ifndef INCLUDE_SRC_SCHEDULEMODEL_H_
#define INCLUDE_SRC_SCHEDULEMODEL_H_

#include <QAbstractListModel>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlTableModel>

class Schedule_Model : public QAbstractListModel {
    Q_OBJECT
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
        statusRole,
        seenRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;

  public slots:
    void setFilter(int week);

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    int m_week;
};

#endif // INCLUDE_SRC_SCHEDULEMODEL_H_
