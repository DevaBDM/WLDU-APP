#include "CacheManager/CacheManager.h"
#include <QCryptographicHash>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <qnetworkreply.h>

CacheManager::CacheManager(QObject *parent)
    : QObject{parent}, m_subHost{}, m_progress{false}, m_host{}, m_savePath{} {
    connect(&m_nm, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
        emit done(this);
        reply->deleteLater();
    });
    connect(this, &CacheManager::hostChanged, this, &CacheManager::reFetch);
    connect(this, &CacheManager::subHostChanged, this, &CacheManager::reFetch);
    setNetworkStatus(NetworkStatus::Waiting);
}

CacheManager::CacheManager(const QString &subHost, const QString &host,
                           const QString &savePath, QObject *parent)
    : CacheManager{parent} {
    m_subHost = subHost;
    m_savePath = savePath.isEmpty() ? "." : savePath;
    setHost(host);
}

void CacheManager::reFetch() {
    if (m_host.isEmpty() || m_subHost.isEmpty() || progress())
        return;
    setProgress(true);
    setNetworkStatus(NetworkStatus::Fetching);
    update();
};

void CacheManager::update() {
    prepareDir();
    QFile file(m_savePath + "/" + m_subHost + ".db");
    if (file.exists())
        file.open(QFile::ReadOnly);
    QNetworkRequest nq{QUrl{m_host + "/" + m_subHost + ".json"}};
    QNetworkReply *reply = m_nm.get(nq);

    connect(reply, &QNetworkReply::finished,
            [&, reply, hash = fileHash(file)]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QJsonDocument jsonDoc{
                        QJsonDocument::fromJson(reply->readAll())};
                    QJsonObject json{jsonDoc.object()};

                    QString nHash{json.value("hash").toString()};
                    if (nHash == hash) {
                        setNetworkStatus(NetworkStatus::Connected);
                        setProgress(false);
                        emit uptodate(this);
                    } else {
                        setNetworkStatus(NetworkStatus::Downloading);
                        upgrade();
                    }
                }
            });

    connectNetworkStatus(reply);
    file.close();
}

void CacheManager::upgrade() {
    setProgress(true);
    QNetworkRequest nq{QUrl{m_host + "/" + m_subHost + ".db"}};
    QNetworkReply *reply = m_nm.get(nq);
    connect(reply, &QNetworkReply::finished, this, [&, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            return;
        QFile file(m_savePath + "/" + m_subHost + ".db");
        file.open(QFile::WriteOnly);
        file.write(reply->readAll());
        file.close();
        setNetworkStatus(NetworkStatus::Connected);
        emit upgraded(this);
        update();
    });
    connectNetworkStatus(reply);
}

QString CacheManager::fileHash(QFile &file) {
    if (!file.isOpen())
        return {};
    QCryptographicHash hash{QCryptographicHash::Sha1};
    hash.addData(&file);
    return hash.result().toHex();
}

CacheManager::NetworkStatus CacheManager::networkStatus() const {
    return m_networkStatus;
}

void CacheManager::setNetworkStatus(NetworkStatus message) {
    if (message == m_networkStatus)
        return;
    m_networkStatus = message;
    emit networkStatusChanged();
}

QString CacheManager::networkMessage(NetworkStatus id) const {
    static QHash<NetworkStatus, QString> message{
        {NetworkStatus::Fetching, "Fetching..."},
        {NetworkStatus::Connecting, "Connecting..."},
        {NetworkStatus::Requesting, "Requesting..."},
        {NetworkStatus::Redirected, "Redirected"},
        {NetworkStatus::Receiving, "Receiving..."},
        {NetworkStatus::Connected, "Connected"},
        {NetworkStatus::Downloading, "Downloading..."},
        {NetworkStatus::Waiting, "Waitting for network..."},
        {NetworkStatus::Error, "Network error"}};

    return message[id];
}

bool CacheManager::progress() const { return m_progress; }
void CacheManager::setProgress(bool progress) {
    if (progress == m_progress)
        return;
    m_progress = progress;
    emit progressChanged();
}

void CacheManager::setHost(const QString &host) {
    if (m_host == host)
        return;
    m_host = host;
    emit hostChanged();
}

void CacheManager::setSubHost(const QString &subHost, const QString &host) {
    if (m_subHost == subHost && m_host == host)
        return;
    m_subHost = subHost;
    m_host = host;
    emit subHostChanged();
}

void CacheManager::connectNetworkStatus(QNetworkReply *reply) {
    connect(reply, &QNetworkReply::socketStartedConnecting,
            [&]() { setNetworkStatus(NetworkStatus::Connecting); });
    connect(reply, &QNetworkReply::requestSent,
            [&]() { setNetworkStatus(NetworkStatus::Requesting); });
    connect(reply, &QNetworkReply::redirected,
            [&]() { setNetworkStatus(NetworkStatus::Redirected); });
    connect(reply, &QNetworkReply::metaDataChanged,
            [&]() { setNetworkStatus(NetworkStatus::Receiving); });
    connect(reply, &QNetworkReply::errorOccurred,
            [=](QNetworkReply::NetworkError error) {
                switch (error) {
                case QNetworkReply::HostNotFoundError:
                    setNetworkStatus(NetworkStatus::Waiting);
                    break;
                default:
                    setNetworkStatus(NetworkStatus::Error);
                }
                setProgress(false);
                emit errorOccurred(error);
            });
    connect(this, &CacheManager::hostChanged, reply, &QNetworkReply::abort);
    connect(this, &CacheManager::subHostChanged, reply, &QNetworkReply::abort);
    //
}

void CacheManager::prepareDir() {
    if (m_subHost.isEmpty() || m_savePath.isEmpty())
        return;
    QDir{}.mkpath(QFileInfo{m_savePath + "/" + m_subHost}.absolutePath());
}

const QString CacheManager::cachedPath() const {
    return m_savePath + "/" + m_subHost + ".db";
}
