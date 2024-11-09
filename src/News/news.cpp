#include "News/news.h"
#include <QDir>

NewsModel::NewsModel(QObject *parent)
    : QAbstractListModel{parent},
      m_db{QSqlDatabase::addDatabase("QSQLITE", "News")},
      m_sqlTable(parent, m_db), m_filter{}, m_mainCache(nullptr),
      m_path("News/news") {
    QDir{}.mkpath("./News");
    prepareModelDB();
    setCache(new CacheManager{"/" + m_path, this});
    connect(Cache(), &CacheManager::upgraded,
            [&](CacheManager *) { prepareModelDB(); });
    // connect(Cache(), &CacheManager::done, [&](CacheManager *) {
    //     prepareModelDB();
    // });
}

int NewsModel::rowCount(const QModelIndex &parent) const {
    return m_sqlTable.rowCount();
}

QHash<int, QByteArray> NewsModel::roleNames() const {
    static QHash<int, QByteArray> roleNames;

    roleNames[titleRole] = "title";
    roleNames[headerRole] = "header";
    roleNames[mainRole] = "main";
    roleNames[footerRole] = "footer";

    return roleNames;
}

QVariant NewsModel::data(const QModelIndex &index, int role) const {
    int row{index.row()};
    if (0 > row || row > rowCount())
        return QVariant{};
    switch (Role(role)) {
    case titleRole:
        return m_sqlTable.record(row).value("title");
    case headerRole:
        return m_sqlTable.record(row).value("header");
    case mainRole:
        return m_sqlTable.record(row).value("main");
    case footerRole:
        return m_sqlTable.record(row).value("footer");
    }
    return {};
}

bool NewsModel::setFilter() {
    beginResetModel();
    // m_sqlTable.setFilter(m_filter.arg(m_date.toString("yyyy-MM-dd")));
    bool select = m_sqlTable.select();
    endResetModel();
    return select;
}

bool NewsModel::prepareModelDB() {
    if (m_db.isOpen())
        m_db.close();
    m_db.setDatabaseName(m_path + ".db");
    m_db.open();
    m_sqlTable.setTable("NewsModel");
    return setFilter();
}

CacheManager *NewsModel::Cache() const { return m_mainCache; }

void NewsModel::setCache(CacheManager *cache) {
    if (!cache && !m_mainCache || cache == m_mainCache)
        return;

    if (m_mainCache)
        m_mainCache->deleteLater();

    m_mainCache = cache;
    emit CacheChanged();
}

void NewsModel::fetch() {
    if (m_mainCache)
        m_mainCache->reFetch();
};
