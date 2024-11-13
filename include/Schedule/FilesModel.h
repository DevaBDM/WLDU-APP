#ifndef INCLUDE_SCHEDULE_FILESMODEL_H_
#define INCLUDE_SCHEDULE_FILESMODEL_H_

#include "User/DownloadManger.h"
#include "User/Downloader.h"
#include <QAbstractListModel>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>

class Files_Model : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
    Q_PROPERTY(DownloadManger *DownloadManger READ downloadManger NOTIFY
                   downloadMangerChanged)
  public:
    explicit Files_Model(QSqlDatabase &db, QString path,
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

    Q_INVOKABLE Downloader *downloader(int);
    Q_INVOKABLE void setHost(const QString &);

    // properties
    void prepareModelDB();
    void setFilter(int slipID, int scheduleID = 0);

    void setPath(const QString &);

    DownloadManger *downloadManger() const;

  private:
  public slots:

  signals:
    void rowCountChanged();
    void downloadMangerChanged();

  private:
    QSqlDatabase &m_db;
    QSqlTableModel m_sqlTable;
    QString m_filter;
    DownloadManger *m_downloadManager;
    QString m_path;
};

#endif // INCLUDE_SCHEDULE_FILESMODEL_H_
