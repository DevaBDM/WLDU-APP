#include "News/news.h"
#include "Downloader.h"
#include <QDir>
#include <QStandardPaths>

NewsModel::NewsModel(const QString &subHost, const QString &host,
                     const QString &savePath, QObject *parent)
    : QAbstractListModel{parent},
      m_db{QSqlDatabase::addDatabase("QSQLITE", "News")},
      m_sqlTable(parent, m_db), m_sqlTableAttachments{parent, m_db},
      m_mainCache(nullptr), m_host{host},
      m_downloadManager{new DownloadManger{
          "NewsPictures", host + "/News",
          QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
          this}} {
    setCache(new CacheManager{subHost, host, savePath, this});
    prepareModelDB(savePath + "/" + subHost + ".db");
    connect(Cache(), &CacheManager::uptodate,
            [&](CacheManager *cache) { prepareModelDB(cache->cachedPath()); });
    // connect(Cache(), &CacheManager::done, [&](CacheManager *) {
    //     prepareModelDB();
    // });
}

int NewsModel::rowCount(const QModelIndex &parent) const {
    return m_sqlTable.rowCount();
}

QHash<int, QByteArray> NewsModel::roleNames() const {
    static QHash<int, QByteArray> roleNames;

    roleNames[newsIDRole] = "newsID";
    roleNames[titleRole] = "title";
    roleNames[headerRole] = "header";
    roleNames[mainRole] = "main";
    roleNames[footerRole] = "footer";

    return roleNames;
}

QVariant NewsModel::data(const QModelIndex &index, int role) const {
    int row{index.row()};
    if (0 > row || row > rowCount())
        return QVariant{};
    switch (Role(role)) {
    case newsIDRole:
        return m_sqlTable.record(row).value("pk_NewsModel");
    case titleRole:
        return m_sqlTable.record(row).value("title");
    case headerRole:
        return m_sqlTable.record(row).value("header");
    case mainRole:
        return m_sqlTable.record(row).value("main");
    case footerRole:
        return m_sqlTable.record(row).value("footer");
    }
    return {};
}

void NewsModel::prepareModelDB(const QString &path) {
    if (m_db.isOpen())
        m_db.close();
    m_db.setDatabaseName(path);
    m_db.open();
    beginResetModel();
    m_sqlTable.setTable("NewsModel");
    m_sqlTable.select();
    m_pictureList.clear();
    endResetModel();
}

CacheManager *NewsModel::Cache() { return m_mainCache; }

void NewsModel::setCache(CacheManager *cache) {
    if (!cache && !m_mainCache || cache == m_mainCache)
        return;

    if (m_mainCache)
        m_mainCache->deleteLater();

    m_mainCache = cache;
    emit CacheChanged();
}

QStringListModel *NewsModel::hashList(int row) {
    if (m_pictureList.contains(row))
        return m_pictureList.value(row);
    QStringList urlList;
    m_sqlTableAttachments.setTable("NewsAttachments");
    m_sqlTableAttachments.setFilter(QString{"pk_NewsModel = %1"}.arg(row));
    m_sqlTableAttachments.select();
    for (int aRow{0}; aRow < m_sqlTableAttachments.rowCount(); ++aRow) {
        urlList.append(
            m_sqlTableAttachments.record(aRow).value("hash").toString());
        Downloader *downloader = m_downloadManager->download(
            m_sqlTableAttachments.record(aRow).value("name").toString(),
            m_sqlTableAttachments.record(aRow).value("hash").toString());
        downloader->startDownload();
    }
    m_pictureList.insert(row, new QStringListModel{urlList, this});
    return m_pictureList.value(row);
}

void NewsModel::setHost(const QString host) {
    if (m_host == host)
        return;
    m_host = host;
    if (m_downloadManager)
        m_downloadManager->setFullHost(host);
};

Downloader *NewsModel::downloader(const QString &name, const QString &hash) {
    if (!m_downloadManager)
        return nullptr;
    return m_downloadManager->download(name, hash);
}

void NewsModel::clearCache() {
    beginResetModel();
    if (m_downloadManager) {
        m_downloadManager->clearDownload();
    }
    endResetModel();
}
