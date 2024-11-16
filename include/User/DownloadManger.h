#ifndef INCLUDE_USER_DOWNLOADMANGER_H_
#define INCLUDE_USER_DOWNLOADMANGER_H_

#include "User/Downloader.h"
#include <QAbstractListModel>
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

class DownloadManger : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int RowCount READ rowCount NOTIFY rowCountChanged)
  public:
    DownloadManger(const QString &fullHost,
                   QAbstractListModel *parent = nullptr);

    enum Role { fileNameRole = Qt::UserRole + 1, hashRole };
    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE Downloader *download(QString saveName, QString hash,
                                     QString host = {});
    Q_INVOKABLE void deleteFile(int);

    void cacheDownloaded(QString saveName, QString hash);
    bool setFullHost(const QString &);
    Q_INVOKABLE void search(const QString &);

  private:
    void updateDownloadersHost();

  signals:
    void currentHashChanged();
    void rowCountChanged();

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    QString m_savePath;
    QString m_fullHost;

    QNetworkAccessManager m_nm;

    QHash<QString, Downloader *> m_downloaders;
};

#endif // INCLUDE_USER_DOWNLOADMANGER_H_
