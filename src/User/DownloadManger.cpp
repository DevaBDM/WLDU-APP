#include "User/DownloadManger.h"
#include "User/Downloader.h"
#include "constant.h"
#include <QDateTime>
#include <QSqlError>

DownloadManger::DownloadManger(QString subHost, QObject *parent)
    : m_db{QSqlDatabase::addDatabase("QSQLITE", "DownloadCache")},
      m_sqlTable(this, m_db), m_savePath{QStandardPaths::writableLocation(
                                             QStandardPaths::CacheLocation) +
                                         "/DownloadCache"},
      m_downloaders(), m_subHost{subHost}, m_host{Constant::k_hostname} {
    QDir{}.mkpath(m_savePath);
    QSqlQuery m_sql_query{m_db};
    m_db.setDatabaseName(m_savePath + "/DownloadCache.db");
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

Downloader *DownloadManger::download(QString saveName, QString hash,
                                     QString host) {
    if (m_downloaders.contains(hash))
        return m_downloaders.value(hash);
    Downloader *d{
        new Downloader{m_db, m_nm, hash, m_savePath, saveName,
                       host.isEmpty() ? m_host + "/" + m_subHost : host, this}};
    connect(d, &Downloader::done, this,
            [&, d]() { cacheDownloaded(d->saveName(), d->hash()); });
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

void DownloadManger::setSubHost(const QString &host) {
    if (m_subHost == host)
        return;
    m_subHost = host;
    updateDownloadersHost();
}

void DownloadManger::setHost(const QString &host) {
    if (m_host == host)
        return;
    m_host = host;
    updateDownloadersHost();
}

void DownloadManger::updateDownloadersHost() {
    for (auto it = m_downloaders.begin(), end = m_downloaders.end(); it != end;
         ++it) {
        it.value()->setHost(m_host + "/" + m_subHost);
    }
}
