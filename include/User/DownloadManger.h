#ifndef INCLUDE_USER_DOWNLOADMANGER_H_
#define INCLUDE_USER_DOWNLOADMANGER_H_

#include "User/Downloader.h"
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
  public:
    DownloadManger(QString subHost, QObject *parent = nullptr);

    Downloader *download(QString saveName, QString hash, QString host = {});

    void cacheDownloaded(QString saveName, QString hash);
    void setSubHost(const QString &);
    void setHost(const QString &);

  private:
    void updateDownloadersHost();

  signals:
    void currentHashChanged();

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    QString m_savePath;
    QString m_subHost;
    QString m_host;

    QNetworkAccessManager m_nm;

    QHash<QString, Downloader *> m_downloaders;
};

#endif // INCLUDE_USER_DOWNLOADMANGER_H_
