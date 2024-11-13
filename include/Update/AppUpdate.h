#ifndef INCLUDE_UPDATE_APPUPDATE_H_
#define INCLUDE_UPDATE_APPUPDATE_H_

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

class AppUpdate : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool uptodate READ uptodate NOTIFY uptodateChanged)
    Q_PROPERTY(bool downloading READ downloading NOTIFY downloadingChanged)
    Q_PROPERTY(bool downloaded READ downloaded NOTIFY downloadedChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
  public:
    explicit AppUpdate(QObject *parent = nullptr);

    int progress() const;
    bool uptodate() const;
    bool downloading() const;
    bool downloaded() const;
    QString version() const;

    void setProgress(double);
    void setUptodate(bool);
    void setDownloading(bool);
    void setDownloaded(bool);
    void setVersion(const QString &);

  public slots:
    void update();
    void fetch();
    void fetch(QString);

  private:
    void download();
    QString fileHash(QFile &file);
    void clear();

  signals:
    void uptodateChanged();
    void downloadingChanged();
    void downloadedChanged();
    void progressChanged();
    void versionChanged();

    void latestVersion();

  private:
    bool m_uptodate{false};
    bool m_downloading{false};
    bool m_downloaded{false};
    double m_progress;
    QString m_version;
    QString m_host;
    QString m_hash;
    QString m_savePath;
    QNetworkAccessManager m_nm;
    QString m_fileName;
};

#endif // INCLUDE_UPDATE_APPUPDATE_H_
