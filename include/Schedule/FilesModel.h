#ifndef INCLUDE_SCHEDULE_FILESMODEL_H_
#define INCLUDE_SCHEDULE_FILESMODEL_H_

#include <QAbstractListModel>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>

class Files_Model : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
  public:
    explicit Files_Model(QSqlDatabase &db,
                         QAbstractListModel *parent = nullptr);
    enum Role {
        codeRole = Qt::UserRole + 1,
        titleRole,
        slipIDRole,
        scheduleIDRole,
        fileTypeRole,
        fileTypeIDRole,
        fileInfoTypeRole,
        fileInfoTypeIDRole,
        fileNameRole,
        fileHashRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;

    // properties
    void prepareModelDB();
    void setFilter(int slipID, int scheduleID = 0);

  private:
  public slots:

  signals:
    void rowCountChanged();

  private:
    QSqlDatabase &m_db;
    QSqlTableModel m_sqlTable;
    QString m_filter;
};

#endif // INCLUDE_SCHEDULE_FILESMODEL_H_
