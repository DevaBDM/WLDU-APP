#include "Schedule/ScheduleModel.h"
#include <QFile>
#include <QSqlRecord>

Schedule_Model::Schedule_Model(QAbstractListModel *parent)
    : QAbstractListModel{parent},
      m_db{QSqlDatabase::addDatabase("QSQLITE", "Schedule")},
      m_sqlTable(this, m_db), m_currentRow(0), m_date(QDate::currentDate()),
      m_filter{
          "(beginDate <= '%1' OR beginDate IS NULL) AND (expireDate > '%1' "
          "OR expireDate IS NULL) AND weekday = strftime('%w','%1') OR "
          "onceDate = '%1'"},
      m_mainCache(nullptr), m_filesModel{m_db, m_path, this},
      m_slipModel(m_db) {
    connectCurrentRow();
    connect(this, &Schedule_Model::dateChanged, this,
            &Schedule_Model::setFilter);
    connect(this, &Schedule_Model::currentRowChanged, [&]() {
        m_filesModel.setFilter(slipID().toInt(), scheduleID().toInt());
    });
    connect(&m_slipModel, &SlipModel::currentRowChanged,
            [&](int row) { m_filesModel.setFilter(row + 1, 0); });
}

Schedule_Model::Schedule_Model(QString p, QAbstractListModel *parent)
    : Schedule_Model(parent) {
    setPath(p);
}

int Schedule_Model::rowCount(const QModelIndex &parent) const {
    return m_sqlTable.rowCount();
}

QHash<int, QByteArray> Schedule_Model::roleNames() const {
    static QHash<int, QByteArray> roleNames;

    roleNames[startTimeRole] = "startTime";
    roleNames[endTimeRole] = "endTime";
    roleNames[titleRole] = "title";
    roleNames[shortNoteRole] = "shortNote";
    roleNames[teacherNameRole] = "teacherName";
    roleNames[teacherPPRole] = "teacherPP";
    roleNames[descriptionRole] = "description";
    roleNames[typeRole] = "type";
    roleNames[slipIDRole] = "slipID";
    roleNames[scheduleIDRole] = "scheduleID";
    roleNames[statusRole] = "status";
    roleNames[seenRole] = "seen";

    return roleNames;
}

QVariant Schedule_Model::data(const QModelIndex &index, int role) const {
    int row{index.row()};
    if (0 > row || row > rowCount())
        return QVariant{};
    switch (Role(role)) {
    case startTimeRole:
        return m_sqlTable.record(row).value("startTime");
    case endTimeRole:
        return m_sqlTable.record(row).value("endTime");
    case titleRole:
        return m_sqlTable.record(row).value("title");
    case shortNoteRole:
        return m_sqlTable.record(row).value("shortNote");
    case teacherNameRole:
        return m_sqlTable.record(row).value("teacherName");
    case descriptionRole:
        return m_sqlTable.record(row).value("note");
    case typeRole:
        return m_sqlTable.record(row).value("schedule_type");
    case slipIDRole:
        return m_sqlTable.record(row).value("pk_Slip");
    case scheduleIDRole:
        return m_sqlTable.record(row).value("pk_Schedule");
    case teacherPPRole:
        return "qrc:/qt/qml/WLDU/assets/icons/teacher.svg";
    case statusRole:
    case seenRole:
        break;
    }
    return {};
}

bool Schedule_Model::setFilter() {
    beginResetModel();
    m_sqlTable.setFilter(m_filter.arg(m_date.toString("yyyy-MM-dd")));
    bool select = m_sqlTable.select();
    endResetModel();
    return select;
}

void Schedule_Model::nextDay() {
    m_date = m_date.addDays(1);
    emit dateChanged();
}
void Schedule_Model::previousDay() {
    m_date = m_date.addDays(-1);
    emit dateChanged();
}

int Schedule_Model::currentRow() const { return m_currentRow; }

void Schedule_Model::setCurrentRow(int row) {
    m_currentRow = row;
    emit currentRowChanged(row);
}

QVariant Schedule_Model::startTime() const {
    return data(index(m_currentRow), startTimeRole);
}

QVariant Schedule_Model::endTime() const {
    return data(index(m_currentRow), endTimeRole);
}

QVariant Schedule_Model::title() const {
    return data(index(m_currentRow), titleRole);
}

QVariant Schedule_Model::shortNote() const {
    return data(index(m_currentRow), shortNoteRole);
}

QVariant Schedule_Model::teacherName() const {
    return data(index(m_currentRow), teacherNameRole);
}

QVariant Schedule_Model::teacherPP() const {
    return data(index(m_currentRow), teacherPPRole);
}

QVariant Schedule_Model::description() const {
    return data(index(m_currentRow), descriptionRole);
}

QVariant Schedule_Model::type() const {
    return data(index(m_currentRow), typeRole);
}

QVariant Schedule_Model::slipID() const {
    return data(index(m_currentRow), slipIDRole);
}

QVariant Schedule_Model::scheduleID() const {
    return data(index(m_currentRow), scheduleIDRole);
}

QVariant Schedule_Model::status() const { return {}; }

QVariant Schedule_Model::seen() const { return {}; }

QVariant Schedule_Model::date() const { return m_date.toString("dd MMMM"); }

QVariant Schedule_Model::weekDay() const {
    int weekDay = m_date.dayOfWeek();
    if (weekDay == 0)
        return {};
    return (weekDay - 1) % 7;
}

void Schedule_Model::setWeekDay(int week) {
    int currentWeek{weekDay().toInt()};
    int wK{week - currentWeek};
    m_date = m_date.addDays(wK);
    emit dateChanged();
}

void Schedule_Model::currentDate() {
    if (m_date == QDate::currentDate())
        return;
    m_date = QDate::currentDate();
    emit dateChanged();
}

void Schedule_Model::fetch() {}

int Schedule_Model::epoch() const { return m_epoch; }

bool Schedule_Model::prepareModelDB() {
    if (m_db.isOpen())
        m_db.close();
    m_db.setDatabaseName(m_path + ".db");
    m_db.open();
    m_filesModel.prepareModelDB();
    m_slipModel.prepareModelDB();
    m_sqlTable.setTable("ScheduleModel");
    return setFilter();
}

void Schedule_Model::connectCurrentRow() {
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::titleChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::startTimeChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::endTime);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::shortNoteChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::teacherNameChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::teacherPPChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::descriptionChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::typeChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::slipIDChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::scheduleIDChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::statusChanged);
    connect(this, &Schedule_Model::currentRowChanged, this,
            &Schedule_Model::seenChanged);
}

void Schedule_Model::setPath(const QString &p) {
    m_filesModel.setPath(p);
    m_path = p + "/schedule";
    prepareModelDB();
    setCache(new CacheManager{"/" + m_path, this});
    connect(Cache(), &CacheManager::upgraded,
            [&](CacheManager *) { prepareModelDB(); });
}

CacheManager *Schedule_Model::Cache() const { return m_mainCache; }

void Schedule_Model::setCache(CacheManager *cache) {
    if (!cache && !m_mainCache || cache == m_mainCache)
        return;

    if (m_mainCache)
        m_mainCache->deleteLater();

    m_mainCache = cache;
    emit CacheChanged();
}

Files_Model *Schedule_Model::FilesModel() { return &m_filesModel; }

SlipModel *Schedule_Model::slip() { return &m_slipModel; }
