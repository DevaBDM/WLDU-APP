#ifndef INCLUDE_NEWS_NEWS_H_
#define INCLUDE_NEWS_NEWS_H_

#include "CacheManager/CacheManager.h"
#include "User/DownloadManger.h"
#include "User/Downloader.h"
#include <QAbstractListModel>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStringListModel>

class NewsModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(CacheManager *Cache READ Cache NOTIFY CacheChanged)
  public:
    explicit NewsModel(const QString &subHost, const QString &host,
                       const QString &savePath, QObject *parent = nullptr);
    enum Role {
        newsIDRole = Qt::UserRole + 1,
        titleRole,
        headerRole,
        mainRole,
        footerRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;

    // properties
    void setCache(CacheManager *);
    void setHost(const QString host);
    Q_INVOKABLE Downloader *downloader(const QString &name,
                                       const QString &hash);
    Q_INVOKABLE QStringListModel *hashList(int row);
    Q_INVOKABLE void clearCache();

    CacheManager *Cache();

  private:
    void prepareModelDB(const QString &);

  public slots:

  signals:
    void CacheChanged();

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    QSqlTableModel m_sqlTableAttachments;
    CacheManager *m_mainCache;
    QString m_host;
    DownloadManger *m_downloadManager;
    QMap<int, QStringListModel *> m_pictureList;
};

#endif // INCLUDE_NEWS_NEWS_H_
