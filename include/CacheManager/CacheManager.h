#ifndef INCLUDE_CACHEMANAGER_CACHEMANAGER_H_
#define INCLUDE_CACHEMANAGER_CACHEMANAGER_H_

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
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
        Fetching,
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

    explicit CacheManager(const QString &subHost, const QString &host,
                          const QString &savePath = QString{},
                          QObject *parent = nullptr);

    Q_INVOKABLE void reFetch();

    void setHost(const QString &);
    void setSubHost(const QString &, const QString &host = {});

    Q_INVOKABLE QString networkMessage(NetworkStatus) const;
    NetworkStatus networkStatus() const;
    bool progress() const;

    const QString cachedPath() const;

  private:
    void setNetworkStatus(NetworkStatus);
    void setProgress(bool);
    void connectNetworkStatus(QNetworkReply *);

    void update();
    void upgrade();
    QString fileHash(QFile &file);
    void prepareDir();

  signals:
    void done(CacheManager *cache = nullptr);
    void upgraded(CacheManager *cache = nullptr);
    void uptodate(CacheManager *cache = nullptr);
    void errorOccurred(QNetworkReply::NetworkError);
    void networkStatusChanged();
    void progressChanged();

    void hostChanged();
    void subHostChanged();

  private:
    QNetworkAccessManager m_nm;
    QString m_host;
    QString m_subHost;
    NetworkStatus m_networkStatus;
    QString m_savePath;
    bool m_progress;
};

#endif // INCLUDE_CACHEMANAGER_CACHEMANAGER_H_
