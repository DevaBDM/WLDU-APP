#ifndef INCLUDE_NEWS_NEWS_H_
#define INCLUDE_NEWS_NEWS_H_

#include "CacheManager/CacheManager.h"
#include <QAbstractListModel>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlTableModel>

class NewsModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(CacheManager *Cache READ Cache NOTIFY CacheChanged)
  public:
    explicit NewsModel(const QString &subHost, const QString &host,
                       const QString &savePath, QObject *parent = nullptr);
    enum Role {
        titleRole = Qt::UserRole + 1,
        headerRole,
        mainRole,
        footerRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;

    // properties
    void setCache(CacheManager *);

    CacheManager *Cache();

  private:
    void prepareModelDB(const QString &);

  public slots:

  signals:
    void CacheChanged();

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    CacheManager *m_mainCache;
};

#endif // INCLUDE_NEWS_NEWS_H_
