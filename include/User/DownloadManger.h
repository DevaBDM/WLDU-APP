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
    DownloadManger(QObject *parent = nullptr);

    Downloader *download(QString host, QString saveName, QString hash);

    void cacheDownloaded(QString saveName, QString hash);

  signals:
    void currentHashChanged();

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    QString m_path;

    QNetworkAccessManager m_nm;

    QHash<QString, Downloader *> m_downloaders;
};

#endif // INCLUDE_USER_DOWNLOADMANGER_H_
