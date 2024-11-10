#ifndef INCLUDE_USER_DOWNLOADMANGER_H_
#define INCLUDE_USER_DOWNLOADMANGER_H_

#include <QCryptographicHash>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>

class DownloadManger : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool downloading READ downloading NOTIFY downloadingChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString currentHash READ currentHash NOTIFY currentHashChanged)
  public:
    DownloadManger(QObject *parent = nullptr);

    void download(QString host, QString saveName, QString hash);

    void downloaded(QString saveName, QString hash);

    QString fileHash(QFile &file);

    bool downloading() const;
    int progress() const;
    QString currentHash() const;

    void setDownloading(bool downloading);
    void setProgress(double);
    void setCurrentHash(QString);

  signals:
    void downloadingChanged();
    void progressChanged();
    void currentHashChanged();

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    QString m_path;

    QNetworkAccessManager m_nm;
    bool m_downloading;
    double m_progress;
    QString m_currentHash;
};

#endif // INCLUDE_USER_DOWNLOADMANGER_H_
