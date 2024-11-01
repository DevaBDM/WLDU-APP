#include "Schedule/ScheduleModel.h"

Schedule_Model::Schedule_Model(QObject *parent)
    : QAbstractListModel(parent),
      m_db{QSqlDatabase::addDatabase("QSQLITE", "Schedule")},
      m_sqlTable(parent, m_db) {
    m_db.setDatabaseName("schedule.db");
    m_db.open();
    m_sqlTable.setTable("ScheduleModel");
    setFilter(0);
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
        return m_sqlTable.record(row).value("description");
    case typeRole:
        return m_sqlTable.record(row).value("schedule_type");
    case slipIDRole:
        return m_sqlTable.record(row).value("pk_Slip");
    case teacherPPRole:
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
