#include "Schedule/SlipModel.h"
#include <QSqlRecord>

SlipModel::SlipModel(QSqlDatabase &db, QAbstractListModel *parent)
    : m_db{db}, m_sqlTable(this, m_db) {
    connectCurrentRow();
    prepareModelDB();
}

int SlipModel::rowCount(const QModelIndex &parent) const {
    return m_sqlTable.rowCount();
}

QHash<int, QByteArray> SlipModel::roleNames() const {
    static QHash<int, QByteArray> roleNames;

    roleNames[slipIDRole] = "slipID";
    roleNames[codeRole] = "code";
    roleNames[titleRole] = "title";
    roleNames[ECTsRole] = "ECTs";
    roleNames[lectureHRole] = "LectureHR";
    roleNames[tutorHRole] = "tutorHR";
    roleNames[lab_practiceHRole] = "lab_practiceHR";
    roleNames[home_studyHRole] = "home_studyHR";
    roleNames[descriptionRole] = "description";

    return roleNames;
}

QVariant SlipModel::data(const QModelIndex &index, int role) const {
    int row{index.row()};
    if (0 > row || row > rowCount())
        return QVariant{};
    switch (Role(role)) {
    case slipIDRole:
        return m_sqlTable.record(row).value("slipID");
    case codeRole:
        return m_sqlTable.record(row).value("code");
    case titleRole:
        return m_sqlTable.record(row).value("title");
    case ECTsRole:
        return m_sqlTable.record(row).value("ECTs");
    case lectureHRole:
        return m_sqlTable.record(row).value("LectureHR");
    case tutorHRole:
        return m_sqlTable.record(row).value("tutorHR");
    case lab_practiceHRole:
        return m_sqlTable.record(row).value("lab_practiceHR");
    case home_studyHRole:
        return m_sqlTable.record(row).value("home_studyHR");
    case descriptionRole:
        return m_sqlTable.record(row).value("description");
    }
    return {};
}

void SlipModel::prepareModelDB() {
    beginResetModel();
    m_sqlTable.setTable("Slip");
    m_sqlTable.select();
    endResetModel();
}

int SlipModel::currentRow() const { return m_currentRow; }

void SlipModel::setCurrentRow(int row) {
    m_currentRow = row;
    emit currentRowChanged(row);
}

QVariant SlipModel::title() const {
    return data(index(m_currentRow), codeRole);
}

QVariant SlipModel::shortNote() const {
    return data(index(m_currentRow), titleRole);
}

QVariant SlipModel::description() const {
    return data(index(m_currentRow), ECTsRole).toString() + " ECTs\n" +
           data(index(m_currentRow), lectureHRole).toString() +
           " Lecture hour\n" +
           data(index(m_currentRow), tutorHRole).toString() +
           " tutorial hour\n" +
           data(index(m_currentRow), lab_practiceHRole).toString() +
           " lab practice hour\n" +
           data(index(m_currentRow), home_studyHRole).toString() +
           " home study hour\n" +
           data(index(m_currentRow), descriptionRole).toString();
}

void SlipModel::connectCurrentRow() {
    connect(this, &SlipModel::currentRowChanged, this,
            &SlipModel::titleChanged);
    connect(this, &SlipModel::currentRowChanged, this,
            &SlipModel::shortNoteChanged);
    // connect(this, &SlipModel::currentRowChanged, this,
    //         &SlipModel::teacherNameChanged);
    // connect(this, &SlipModel::currentRowChanged, this,
    //         &SlipModel::teacherPPChanged);
    connect(this, &SlipModel::currentRowChanged, this,
            &SlipModel::descriptionChanged);
}
