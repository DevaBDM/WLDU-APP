#include "Register/Register.h"

Register::Register(QObject *parent)
    : QObject{parent}, m_db{QSqlDatabase::addDatabase("QSQLITE", "Register")},
      m_sqlTable{parent, m_db} {
    m_db.setDatabaseName("Register/register.db");
    m_db.open();
    fill("program_type", m_program_type);
    fill("Addmission_type", m_addmission_type);
    fill("Department", m_department);
    fill("Department_stream", m_department_stream);
}

void Register::fill(QString tableName, QStringList &member) {
    m_sqlTable.setTable(tableName);
    m_sqlTable.select();
    for (int i{0}; i < m_sqlTable.rowCount(); ++i) {
        member.append(m_sqlTable.record(i).value("name").toString());
    }
}

QStringList Register::program_type() const { return m_program_type; }
QStringList Register::addmission_type() const { return m_addmission_type; }
QStringList Register::department() const { return m_department; }
QStringList Register::department_stream() const { return m_department_stream; }
