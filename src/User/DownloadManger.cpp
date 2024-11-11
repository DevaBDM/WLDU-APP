#include "User/DownloadManger.h"
#include "User/Downloader.h"
#include <QDateTime>
#include <QSqlError>

DownloadManger::DownloadManger(QObject *parent)
    : m_db{QSqlDatabase::addDatabase("QSQLITE", "DownloadCache")},
      m_sqlTable(this, m_db),
      m_path{QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
             "/DownloadCache"},
      m_downloaders() {
    QDir{}.mkpath(m_path);
    QSqlQuery m_sql_query{m_db};
    m_db.setDatabaseName(m_path + "/DownloadCache.db");
    m_db.open();
    m_sql_query.exec(
        "CREATE TABLE IF NOT EXISTS DownloadCache(pk_DownloadCache INTEGER "
        "PRIMARY KEY AUTOINCREMENT,hash TEXT UNIQUE,fileName TEXT,update_date "
        "TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP)");

    m_sqlTable.setTable("DownloadCache");
    m_sqlTable.select();

    connect(&m_nm, &QNetworkAccessManager::finished,
            [](QNetworkReply *reply) { reply->deleteLater(); });
}

Downloader *DownloadManger::download(QString host, QString saveName,
                                     QString hash) {
    if (m_downloaders.contains(hash))
        return m_downloaders.value(hash);
    Downloader *d{
        new Downloader{m_db, m_nm, hash, m_path, saveName, host, this}};
    connect(d, &Downloader::done, this,
            [&, saveName, hash]() { cacheDownloaded(saveName, hash); });
    m_downloaders[hash] = d;
    return d;
}

void DownloadManger::cacheDownloaded(QString saveName, QString hash) {
    QSqlRecord r{m_sqlTable.record()};
    r.setValue("fileName", saveName);
    r.setValue("hash", hash);
    r.setValue("update_date",
               QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss"));
    m_sqlTable.insertRecord(-1, r);
    m_sqlTable.select();
}
