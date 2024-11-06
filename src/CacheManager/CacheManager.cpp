#include "CacheManager/CacheManager.h"

CacheManager::CacheManager(QObject *parent) : QObject{parent}, m_p{} {
    connect(&m_nm, &QNetworkAccessManager::finished,
            [](QNetworkReply *reply) { reply->deleteLater(); });
}

CacheManager::CacheManager(QString p, QObject *parent) : CacheManager{parent} {
    m_p = p;
    update(p);
}

void CacheManager::fetchRegister() { update("/Register/register"); }

void CacheManager::update(QString path) {
    QFile file("." + path + ".db");
    if (file.exists())
        file.open(QFile::ReadOnly);
    QNetworkRequest nq{QUrl{"http://localhost/WLDU" + path + ".json"}};
    QNetworkReply *reply = m_nm.get(nq);
    connect(reply, &QNetworkReply::finished,
            [&, reply, hash = fileHash(file), path]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QJsonDocument jsonDoc{
                        QJsonDocument::fromJson(reply->readAll())};
                    QJsonObject json{jsonDoc.object()};

                    QString nHash{json.value("hash").toString()};
                    if (nHash == hash) {
                        emit done(this);
                    } else {
                        upgrade(path);
                    }
                }
            });

    connect(reply, &QNetworkReply::errorOccurred,
            [=](QNetworkReply::NetworkError error) { qDebug() << error; });
    file.close();
}

void CacheManager::upgrade(QString path) {
    QNetworkRequest nq{QUrl{"http://localhost/WLDU" + path + ".db"}};
    QNetworkReply *reply = m_nm.get(nq);
    connect(reply, &QNetworkReply::finished, this, [&, reply, path]() {
        if (reply->error() != QNetworkReply::NoError)
            return;
        QFile file("." + path + ".db");
        file.open(QFile::WriteOnly);
        file.write(reply->readAll());
        file.close();
        update(path);
    });
}

QString CacheManager::fileHash(QFile &file) {
    if (!file.isOpen())
        return {};
    QCryptographicHash hash{QCryptographicHash::Sha1};
    hash.addData(&file);
    return hash.result().toHex();
}
