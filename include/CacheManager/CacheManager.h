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
#include <QQmlEngine>

class CacheManager : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(NetworkStatus networkStatus READ networkStatus NOTIFY
                   networkStatusChanged)
    Q_PROPERTY(bool progress READ progress NOTIFY progressChanged)
  public:
    enum class NetworkStatus {
        Connecting,
        Requesting,
        Redirected,
        Receiving,
        Connected,
        Downloading,
        Waiting,
        Error
    };
    Q_ENUM(NetworkStatus)

    explicit CacheManager(QObject *parent = nullptr);

    explicit CacheManager(QString p, QObject *parent = nullptr);

    Q_INVOKABLE void reFetch();

    Q_INVOKABLE QString networkMessage(NetworkStatus) const;
    NetworkStatus networkStatus() const;
    bool progress() const;

  private:
    void setNetworkStatus(NetworkStatus);
    void setProgress(bool);

    void update(QString path);
    void upgrade(QString path);
    QString fileHash(QFile &file);

  signals:
    void done(CacheManager *cache = nullptr);
    void upgraded(CacheManager *cache = nullptr);
    void networkStatusChanged();
    void progressChanged();

  private:
    QNetworkAccessManager m_nm;
    QString m_p;
    NetworkStatus m_networkStatus;
    bool m_progress;
};

#endif // INCLUDE_CACHEMANAGER_CACHEMANAGER_H_
