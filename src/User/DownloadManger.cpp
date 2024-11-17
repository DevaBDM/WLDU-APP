#include "User/DownloadManger.h"
#include "User/Downloader.h"
#include <QDateTime>
#include <QSqlError>
#include <QStandardPaths>

DownloadManger::DownloadManger(const QString &dbName, const QString &fullHost,
                               QAbstractListModel *parent)
    : DownloadManger{
          dbName, fullHost,
          QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
          parent} {}

DownloadManger::DownloadManger(const QString &dbName, const QString &fullHost,
                               const QString &savePath,
                               QAbstractListModel *parent)
    : QAbstractListModel{parent},
      m_db{QSqlDatabase::addDatabase("QSQLITE", dbName)},
      m_sqlTable(this, m_db), m_savePath{savePath + "/" + dbName},
      m_downloaders(), m_fullHost{fullHost} {
    connect(&m_nm, &QNetworkAccessManager::finished,
            [](QNetworkReply *reply) { reply->deleteLater(); });
    prepareModelDB();
}

void DownloadManger::prepareModelDB() {
    QDir{}.mkpath(m_savePath);
    QSqlQuery m_sql_query{m_db};
    m_db.setDatabaseName(m_savePath + "/DownloadCache.db");
    m_db.open();
    m_sql_query.exec(
        "CREATE TABLE IF NOT EXISTS DownloadCache(pk_DownloadCache INTEGER "
        "PRIMARY KEY AUTOINCREMENT,hash TEXT UNIQUE,fileName "
        "TEXT,update_date "
        "TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP)");

    m_sqlTable.setTable("DownloadCache");
    m_sqlTable.select();
}

int DownloadManger::rowCount(const QModelIndex &parent) const {
    return m_sqlTable.rowCount();
}

QHash<int, QByteArray> DownloadManger::roleNames() const {
    static QHash<int, QByteArray> roleNames;

    roleNames[fileNameRole] = "fileName";
    roleNames[hashRole] = "hash";

    return roleNames;
}

QVariant DownloadManger::data(const QModelIndex &index, int role) const {
    int row{index.row()};
    if (0 > row || row > rowCount())
        return QVariant{};
    switch (Role(role)) {
    case fileNameRole:
        return m_sqlTable.record(row).value("fileName");
    case hashRole:
        return m_sqlTable.record(row).value("hash");
        break;
    }
    return {};
}

Downloader *DownloadManger::download(QString saveName, QString hash,
                                     QString host) {
    if (m_downloaders.contains(hash))
        return m_downloaders.value(hash);
    Downloader *d{new Downloader{m_db, m_nm, hash, m_savePath, saveName,
                                 host.isEmpty() ? m_fullHost : host, this}};
    connect(d, &Downloader::done, this,
            [&, d]() { cacheDownloaded(d->saveName(), d->hash()); });
    m_downloaders[hash] = d;
    return d;
}

void DownloadManger::cacheDownloaded(QString saveName, QString hash) {
    beginInsertRows(QModelIndex{}, rowCount(), rowCount());
    QSqlRecord r{m_sqlTable.record()};
    r.setValue("fileName", saveName);
    r.setValue("hash", hash);
    r.setValue("update_date",
               QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss"));
    m_sqlTable.insertRecord(-1, r);
    m_sqlTable.select();
    endInsertRows();
}

bool DownloadManger::setFullHost(const QString &fullHost) {
    if (m_fullHost == fullHost)
        return false;
    m_fullHost = fullHost;
    updateDownloadersHost();
    return true;
}

void DownloadManger::updateDownloadersHost() {
    for (auto it = m_downloaders.begin(), end = m_downloaders.end(); it != end;
         ++it) {
        it.value()->setHost(m_fullHost);
    }
}

void DownloadManger::deleteFile(int row) {
    if (!QFile{m_savePath + "/" +
               m_sqlTable.record(row).value("fileName").toString()}
             .remove())
        return;
    QString hash = m_sqlTable.record(row).value("hash").toString();
    if (m_downloaders.contains(hash))
        m_downloaders.value(hash)->setDownloaded(false);
    beginRemoveRows(QModelIndex{}, row, row);
    m_sqlTable.removeRows(row, 1);

    bool rm = m_sqlTable.submit();
    m_sqlTable.select();

    endRemoveRows();
    emit rowCountChanged();
}

void DownloadManger::search(const QString &str) {
    beginResetModel();
    m_sqlTable.setFilter(QString("fileName LIKE '%%%1%%'").arg(str));
    m_sqlTable.filter();
    m_sqlTable.select();
    endResetModel();
}

void DownloadManger::clearDownload() {
    if (m_db.isOpen())
        m_db.close();
    for (auto it = m_downloaders.begin(), end = m_downloaders.end(); it != end;
         ++it) {
        it.value()->deleteLater();
    }
    m_downloaders.clear();
    QDir{m_savePath}.removeRecursively();
    prepareModelDB();
}
