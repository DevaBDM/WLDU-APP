#ifndef INCLUDE_USER_DOWNLOADER_H_
#define INCLUDE_USER_DOWNLOADER_H_

#include <QCryptographicHash>
#include <QFile>
#include <QHash>
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>
#include <QSqlDatabase>

class Downloader : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY( bool waitting READ waitting WRITE setWaitting NOTIFY waittingChanged)
    Q_PROPERTY(bool downloading READ downloading WRITE setDownloading NOTIFY downloadingChanged)
    Q_PROPERTY( int progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(bool downloaded READ downloaded WRITE setDownloaded NOTIFY downloadedChanged)

    Q_PROPERTY(QString hash READ hash WRITE setHash NOTIFY hashChanged)
  public:
    Downloader(QSqlDatabase &db, QNetworkAccessManager &nm, QString hash,
               QString path, QString saveName, QString host,
               QObject *parent = nullptr);

    Q_INVOKABLE void startDownload();

    QString fileHash(QFile &file);

    bool waitting() const;
    bool downloading() const;
    int progress() const;
    bool downloaded() const;

    QString hash() const;

    void setWaitting(bool newWaitting);
    void setDownloading(bool newDownloading);
    void setProgress(double newProgress);
    void setDownloaded(bool newDownloaded);

    void setHash(const QString &newHash);

  signals:
    void waittingChanged();
    void downloadingChanged();
    void progressChanged();
    void downloadedChanged();
    void hashChanged();
    void done();

  private:
    QSqlDatabase &m_db;

    QNetworkAccessManager &m_nm;
    bool m_waitting;
    bool m_downloading;
    double m_progress;
    bool m_downloaded;

    QString m_hash;
    QString m_path;
    QString m_saveName;
    QString m_host;
};

#endif // INCLUDE_USER_DOWNLOADER_H_
