#include "Schedule/ScheduleModel.h"

Schedule_Model::Schedule_Model(QObject *parent)
    : QAbstractListModel(parent),
      m_db{QSqlDatabase::addDatabase("QSQLITE", "Schedule")},
      m_sqlTable(parent, m_db), m_currentRow(0) {
    m_db.setDatabaseName("schedule.db");
    m_db.open();
    m_sqlTable.setTable("ScheduleModel");
    setFilter(0);
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
        return "qrc:/qt/qml/WLDU/assets/icons/circle-user.svg";
    case statusRole:
    case seenRole:
        break;
    }
    return {};
}

void Schedule_Model::setFilter(int week) {
    if (m_week == week)
        return;
    m_week = week;
    beginResetModel();
    m_sqlTable.setFilter("weekday = " + QString::number(week));
    m_sqlTable.select();
    endResetModel();
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
