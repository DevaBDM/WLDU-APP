#include "User/Downloader.h"
#include <QCryptographicHash>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSqlQuery>
#include <QUrl>

Downloader::Downloader(QSqlDatabase &db, QNetworkAccessManager &nm,
                       QString hash, QString path, QString saveName,
                       QString host, QObject *parent)
    : QObject{parent}, m_nm{nm}, m_db{db}, m_hash{hash}, m_path(path),
      m_saveName{saveName}, m_host{host}, m_waitting(false),
      m_downloading(false), m_progress(0), m_downloaded(false) {

    QSqlQuery m_sql_query{m_db};
    m_sql_query.prepare("SELECT * from DownloadCache where hash = ?");
    m_sql_query.addBindValue(hash);
    m_sql_query.exec();
    if (m_sql_query.next()) {
        m_saveName = m_sql_query.value("fileName").toString();
        setDownloaded(true);
    }
    m_sql_query.clear();
    m_sql_query.finish();
}

bool Downloader::waitting() const { return m_waitting; }
bool Downloader::downloading() const { return m_downloading; }
int Downloader::progress() const { return m_progress * 100; }
bool Downloader::downloaded() const { return m_downloaded; }
QString Downloader::hash() const { return m_hash; }

void Downloader::setWaitting(bool newWaitting) {
    if (m_waitting == newWaitting)
        return;
    m_waitting = newWaitting;
    emit waittingChanged();
}

void Downloader::setDownloading(bool newDownloading) {
    if (m_downloading == newDownloading)
        return;
    m_downloading = newDownloading;
    emit downloadingChanged();
}

void Downloader::setProgress(double newProgress) {
    if (qFuzzyCompare(m_progress, newProgress))
        return;
    m_progress = newProgress;
    emit progressChanged();
}

void Downloader::setDownloaded(bool newDownloaded) {
    if (m_downloaded == newDownloaded)
        return;
    m_downloaded = newDownloaded;
    emit downloadedChanged();
}

void Downloader::setHash(const QString &newHash) {
    if (m_hash == newHash)
        return;
    m_hash = newHash;
    emit hashChanged();
}

QString Downloader::fileHash(QFile &file) {
    if (!file.isOpen())
        return {};
    QCryptographicHash hash{QCryptographicHash::Sha1};
    hash.addData(&file);
    return hash.result().toHex();
}

void Downloader::startDownload() {
    if (downloaded()) {
        qDebug() << "Opening file" << m_saveName;
        return;
    } else if (downloading())
        return;
    setWaitting(false);
    setDownloading(true);
    QNetworkReply *reply =
        m_nm.get(QNetworkRequest{QUrl{m_host + "/" + m_hash}});
    QFile *file = new QFile(m_path + "/" + m_saveName);
    while (file->exists()) {
        m_saveName = m_saveName.prepend("_");
        file->setFileName(m_path + "/" + m_saveName);
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

    connect(reply, &QNetworkReply::finished, [&, reply, file]() {
        file->close();
        file->deleteLater();
        QFile f(m_path + "/" + m_saveName);
        f.open(QFile::ReadOnly);
        if (fileHash(f) == m_hash) {
            f.close();
            emit done();
            setWaitting(false);
            setDownloading(false);
            setDownloaded(true);
        } else {
            f.close();
            QFile{m_path + "/" + m_saveName}.remove();
            setWaitting(false);
            setDownloading(false);
            setDownloaded(false);
            // startDownload();
        }
    });
    connect(reply, &QNetworkReply::readyRead,
            [&, reply, file]() { file->write(reply->readAll()); });
    connect(reply, &QNetworkReply::downloadProgress,
            [&, reply](double r, double t) { setProgress(r / t); });
}

void Downloader::setHost(const QString &host) {
    if (m_host == host)
        return;
    m_host = host;
}

QString Downloader::saveName() const { return m_saveName; };
