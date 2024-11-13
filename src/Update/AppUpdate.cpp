#include "Update/AppUpdate.h"
#include "constant.h"
#include <QCryptographicHash>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace Constant {
#ifdef ARCHTECTURE_ARM64_V8A
const QString k_arch{"arm64"};
#elif defined(ARCHTECTURE_ARMEABI_V7A)
const QString k_arch{"armeabi_v7a"};
#else
const QString k_arch{""};
#endif
} // namespace Constant

AppUpdate::AppUpdate(QObject *parent)
    : QObject{parent}, m_uptodate{true}, m_downloading{false}, m_progress{0},
      m_downloaded{false}, m_version{"v0.4.5"},
      m_host(Constant::k_hostname + "/App"),
      m_savePath{
          QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
          "/App"},
      m_fileName("WLDU_" + Constant::k_arch) {
    QDir{}.mkpath(m_savePath);
    connect(&m_nm, &QNetworkAccessManager::finished,
            [](QNetworkReply *reply) { reply->deleteLater(); });
    fetch();
}

int AppUpdate::progress() const { return m_progress * 100; };
bool AppUpdate::uptodate() const { return m_uptodate; }
bool AppUpdate::downloading() const { return m_downloading; }
bool AppUpdate::downloaded() const { return m_downloaded; }

void AppUpdate::setProgress(double progress) {
    if (m_progress == progress)
        return;
    m_progress = progress;
    emit progressChanged();
}
void AppUpdate::setUptodate(bool uptodate) {
    if (m_uptodate == uptodate)
        return;
    m_uptodate = uptodate;
    emit uptodateChanged();
}
void AppUpdate::setDownloading(bool downloading) {
    if (m_downloading == downloading)
        return;
    m_downloading = downloading;
    emit downloadingChanged();
}
void AppUpdate::setDownloaded(bool downloaded) {
    if (m_downloaded == downloaded)
        return;
    m_downloaded = downloaded;
    emit downloadedChanged();
}

void AppUpdate::update() {
    if (m_downloaded)
        qDebug() << "Install" << m_savePath + "/" + m_fileName + ".apk";
    else
        download();
}

void AppUpdate::fetch() {
    QNetworkReply *reply =
        m_nm.get(QNetworkRequest{QUrl{m_host + "/" + m_fileName + ".json"}});

    connect(reply, &QNetworkReply::finished, [&, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonDoc{QJsonDocument::fromJson(reply->readAll())};
            QJsonObject json{jsonDoc.object()};

            QString version{json.value("version").toString()};
            if (m_version == version) {
                setUptodate(true);
                QFile f{m_savePath + "/" + m_fileName + ".json"};
                if (f.exists())
                    f.remove();
                clear();
                emit latestVersion();
            } else {
                setUptodate(false);
                m_hash = json.value("hash").toString();

                QFile f{m_savePath + "/" + m_fileName + ".apk"};
                f.open(QFile::ReadOnly);
                QFile file{m_savePath + "/" + m_fileName + ".json"};
                file.open(QFile::WriteOnly);
                file.write(jsonDoc.toJson());
                file.close();
                if (fileHash(f) == m_hash) {
                    setDownloaded(true);
                }
                f.close();
            }
        } else {
            QFile file{m_savePath + "/" + m_fileName + ".json"};
            if (!file.exists())
                clear();
            else {
                file.open(QFile::ReadOnly);
                QJsonDocument jsonDoc{QJsonDocument::fromJson(file.readAll())};
                file.close();
                QJsonObject json{jsonDoc.object()};

                QString version{json.value("version").toString()};
                if (m_version == version) {
                    setUptodate(true);
                    file.remove();
                    clear();
                    emit latestVersion();
                } else {
                    m_hash = json.value("hash").toString();

                    QFile f{m_savePath + "/" + m_fileName + ".apk"};
                    if (!f.exists()) {
                        setUptodate(true);
                        file.remove();
                    } else {
                        f.open(QFile::ReadOnly);
                        if (fileHash(f) == m_hash) {
                            setUptodate(false);
                            setDownloaded(true);
                            f.close();
                        } else {
                            setUptodate(true);
                            f.remove();
                            file.remove();
                        }
                    }
                }
            }
        }
    });
};
void AppUpdate::fetch(QString host) {
    if (m_host == host)
        return;
    m_host = host + "/App";
    fetch();
}

void AppUpdate::download() {
    setDownloading(true);
    QNetworkReply *reply =
        m_nm.get(QNetworkRequest{QUrl{m_host + "/" + m_fileName + ".apk"}});

    QFile *file = new QFile(m_savePath + "/" + m_fileName + ".apk");
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
        QFile f{m_savePath + "/" + m_fileName + ".apk"};
        f.open(QFile::ReadOnly);
        if (fileHash(f) == m_hash) {
            f.close();
            setDownloading(false);
            setDownloaded(true);
        } else {
            f.close();
            download();
        }
    });

    connect(reply, &QNetworkReply::readyRead,
            [&, reply, file]() { file->write(reply->readAll()); });
    connect(reply, &QNetworkReply::downloadProgress,
            [&, reply](double r, double t) { setProgress(r / t); });
}

QString AppUpdate::fileHash(QFile &file) {
    if (!file.isOpen())
        return {};
    QCryptographicHash hash{QCryptographicHash::Sha1};
    hash.addData(&file);
    return hash.result().toHex();
}

void AppUpdate::clear() {
    QFile file{m_savePath + "/" + m_fileName + ".apk"};
    if (file.exists())
        file.remove();
}

QString AppUpdate::version() const {
    return m_version + " " + Constant::k_arch;
}

void AppUpdate::setVersion(const QString &version) {
    if (version == m_version)
        return;
    m_version = version;
    emit versionChanged();
}
