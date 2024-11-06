#ifndef INCLUDE_CACHEMANAGER_CACHEMANAGER_H_
#define INCLUDE_CACHEMANAGER_CACHEMANAGER_H_

#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

class CacheManager : public QObject {
    Q_OBJECT
  public:
    explicit CacheManager(QObject *parent = nullptr);

    explicit CacheManager(QString p, QObject *parent = nullptr);

    void fetchRegister();

  private:
    void update(QString path);

    void upgrade(QString path);

    QString fileHash(QFile &file);

  signals:
    void done(CacheManager *cache = nullptr);

  private:
    QNetworkAccessManager m_nm;
    QString m_p;
};

#endif // INCLUDE_CACHEMANAGER_CACHEMANAGER_H_
