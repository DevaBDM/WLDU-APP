#include "CacheManager/CacheManager.h"

CacheManager::CacheManager(QObject *parent)
    : QObject{parent}, m_p{}, m_progress{false} {
    connect(&m_nm, &QNetworkAccessManager::finished,
            [](QNetworkReply *reply) { reply->deleteLater(); });
    setNetworkStatus(NetworkStatus::Waiting);
}

CacheManager::CacheManager(QString p, QObject *parent) : CacheManager{parent} {
    m_p = p;
    update(p);
}

void CacheManager::reFetch() {
    if (m_p.isEmpty() || progress())
        return;
    setNetworkStatus(NetworkStatus::Waiting);
    update(m_p);
};

void CacheManager::update(QString path) {
    QFile file("." + path + ".db");
    if (file.exists())
        file.open(QFile::ReadOnly);
    QNetworkRequest nq{QUrl{"http://localhost/WLDU" + path + ".json"}};
    QNetworkReply *reply = m_nm.get(nq);
    setProgress(true);
    connect(reply, &QNetworkReply::finished,
            [&, reply, hash = fileHash(file), path]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QJsonDocument jsonDoc{
                        QJsonDocument::fromJson(reply->readAll())};
                    QJsonObject json{jsonDoc.object()};

                    QString nHash{json.value("hash").toString()};
                    if (nHash == hash) {
                        setNetworkStatus(NetworkStatus::Connected);
                        setProgress(false);
                        emit done(this);
                    } else {
                        setNetworkStatus(NetworkStatus::Downloading);
                        upgrade(path);
                    }
                }
            });

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
            });
    file.close();
}

void CacheManager::upgrade(QString path) {
    QNetworkRequest nq{QUrl{"http://localhost/WLDU" + path + ".db"}};
    QNetworkReply *reply = m_nm.get(nq);
    setProgress(true);
    connect(reply, &QNetworkReply::finished, this, [&, reply, path]() {
        if (reply->error() != QNetworkReply::NoError)
            return;
        QFile file("." + path + ".db");
        file.open(QFile::WriteOnly);
        file.write(reply->readAll());
        file.close();
        setNetworkStatus(NetworkStatus::Connected);
        emit upgraded(this);
        update(path);
    });
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
            });
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
