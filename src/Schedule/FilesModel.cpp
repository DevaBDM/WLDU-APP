#include "Schedule/FilesModel.h"
#include <QSqlRecord>
#include <QStandardPaths>

Files_Model::Files_Model(QSqlDatabase &db, QString path,
                         QAbstractListModel *parent)
    : QAbstractListModel{parent},
      m_filter("(pk_Slip = %1 or pk_Slip IS NULL) and (pk_Schedule = %2 or "
               "pk_Schedule IS NULL)"),
      m_db(db), m_sqlTable{this, db}, m_downloadManager(nullptr) {}
int Files_Model::rowCount(const QModelIndex &parent) const {
    return m_sqlTable.rowCount();
}

QHash<int, QByteArray> Files_Model::roleNames() const {
    static QHash<int, QByteArray> roleNames;

    roleNames[codeRole] = "code";
    roleNames[titleRole] = "title";
    roleNames[slipIDRole] = "slipID";
    roleNames[scheduleIDRole] = "scheduleID";
    roleNames[fileTypeRole] = "fileType";
    roleNames[fileTypeIDRole] = "fileTypeID";
    roleNames[fileInfoTypeRole] = "fileInfo";
    roleNames[fileInfoTypeIDRole] = "fileInfoID";
    roleNames[fileNameRole] = "fileName";
    roleNames[fileHashRole] = "fileHash";

    return roleNames;
}

QVariant Files_Model::data(const QModelIndex &index, int role) const {
    int row{index.row()};
    if (0 > row || row > rowCount())
        return QVariant{};
    switch (Role(role)) {
    case codeRole:
        return m_sqlTable.record(row).value("code");
    case titleRole:
        return m_sqlTable.record(row).value("title");
    case slipIDRole:
        return m_sqlTable.record(row).value("pk_Slip");
    case scheduleIDRole:
        return m_sqlTable.record(row).value("pk_Schedule");
    case fileTypeRole:
        return m_sqlTable.record(row).value("fileType");
    case fileTypeIDRole:
        return m_sqlTable.record(row).value("pk_FileType");
    case fileInfoTypeRole:
        return m_sqlTable.record(row).value("fileInfo");
    case fileInfoTypeIDRole:
        return m_sqlTable.record(row).value("pk_FileInfo");
    case fileNameRole:
        return m_sqlTable.record(row).value("name");
    case fileHashRole:
        return m_sqlTable.record(row).value("hash");
        break;
    }
    return {};
}

void Files_Model::setFilter(int slipID, int scheduleID) {
    beginResetModel();
    m_sqlTable.setFilter(m_filter.arg(slipID).arg(scheduleID));
    m_sqlTable.select();
    endResetModel();
    emit rowCountChanged();
};
void Files_Model::prepareModelDB() {
    if (!m_db.isOpen())
        return;
    m_sqlTable.setTable("FilesModel");
    setFilter(0, 0);
}

Downloader *Files_Model::downloader(int row) {
    if (0 > row || row > rowCount())
        return nullptr;
    if (!m_downloadManager)
        return nullptr;
    return m_downloadManager->download(
        m_sqlTable.record(row).value("name").toString(),
        m_sqlTable.record(row).value("hash").toString());
}

void Files_Model::setPath(const QString &fullHost,
                          const QString &downloadPath) {
    if (m_downloadManager) {
        if (m_downloadManager->setFullHost(fullHost))
            emit downloadMangerChanged();
    } else {
        m_downloadManager = new DownloadManger{
            "CourseAttachments", fullHost,
            downloadPath,
            this};
        emit downloadMangerChanged();
    }
}

void Files_Model::setHost(const QString &host) {
    if (m_downloadManager)
        m_downloadManager->setFullHost(host);
}

DownloadManger *Files_Model::downloadManger() const {
    return m_downloadManager;
}
