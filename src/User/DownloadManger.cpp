#include "User/DownloadManger.h"
#include <QDateTime>
#include <QSqlError>

DownloadManger::DownloadManger(QObject *parent)
    : m_db{QSqlDatabase::addDatabase("QSQLITE", "DownloadCache")},
      m_sqlTable(this, m_db),
      m_path{QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
             "/DownloadCache"},
      m_downloading(false) {
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

void DownloadManger::download(QString host, QString saveName, QString hash) {
    if (m_downloading)
        return;
    setDownloading(true);
    setCurrentHash(hash);
    QSqlQuery m_sql_query{m_db};
    m_sql_query.prepare("SELECT * from DownloadCache where hash = ?");
    m_sql_query.addBindValue(hash);
    m_sql_query.exec();
    if (m_sql_query.next()) {
        m_sql_query.clear();
        m_sql_query.finish();
        setDownloading(false);
        return;
    }
    m_sql_query.clear();
    m_sql_query.finish();
    setDownloading(true);
    QNetworkReply *reply = m_nm.get(QNetworkRequest{QUrl{host + "/" + hash}});
    QFile *file = new QFile(m_path + "/" + saveName);
    while (file->exists()) {
        saveName = saveName.prepend("_");
        file->setFileName(m_path + "/" + saveName);
        if (file->fileName().size() > 130) {
            reply->abort();
            file->deleteLater();
            delete file;
            setDownloading(false);
            return;
        }
    }
    if (!file->open(QFile::WriteOnly)) {
        reply->abort();
        reply->deleteLater();
        delete file;
        setDownloading(false);
        return;
    }

    connect(reply, &QNetworkReply::finished,
            [&, reply, file, host, saveName, hash]() {
                file->close();
                file->deleteLater();
                QFile f(file->fileName());
                f.open(QFile::ReadOnly);
                if (fileHash(f) == hash) {
                    f.close();
                    setDownloading(false);
                    downloaded(saveName, hash);
                } else {
                    f.close();
                    download(host, saveName, hash);
                }
            });
    connect(reply, &QNetworkReply::readyRead,
            [&, reply, file]() { file->write(reply->readAll()); });
    connect(reply, &QNetworkReply::downloadProgress,
            [&, reply](double r, double t) { setProgress(r / t); });
}

void DownloadManger::downloaded(QString saveName, QString hash) {
    QSqlRecord r{m_sqlTable.record()};
    r.setValue("fileName", saveName);
    r.setValue("hash", hash);
    r.setValue("update_date",
               QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss"));
    m_sqlTable.insertRecord(-1, r);
    m_sqlTable.select();
}

QString DownloadManger::fileHash(QFile &file) {
    if (!file.isOpen())
        return {};
    QCryptographicHash hash{QCryptographicHash::Sha1};
    hash.addData(&file);
    return hash.result().toHex();
}

void DownloadManger::setDownloading(bool downloading) {
    if (m_downloading == downloading)
        return;
    m_downloading = downloading;
    emit downloadingChanged();
}
bool DownloadManger::downloading() const { return m_downloading; }

int DownloadManger::progress() const { return m_progress * 100; };
void DownloadManger::setProgress(double progress) {
    if (m_progress == progress)
        return;
    m_progress = progress;
    emit progressChanged();
}

QString DownloadManger::currentHash() const { return m_currentHash; };
void DownloadManger::setCurrentHash(QString hash) {
    if (m_currentHash == hash)
        return;
    m_currentHash = hash;
    emit currentHashChanged();
}
