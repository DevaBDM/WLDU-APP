#include "Register/Register.h"

Register::Register(QObject *parent)
    : QObject{parent}, m_db{QSqlDatabase::addDatabase("QSQLITE", "Register")},
      m_sqlTable{this, m_db}, m_sqlTableSupported(this, m_db) {
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

bool Register::isSupported(int programIndex, int addmissionIndex, int year,
                           int semester, int section, int departmentIndex,
                           int streamIndex) {
    m_sqlTableSupported.setTable("SupportedMaterial");
    QString where{"pk_Program_type = %1 AND pk_Addmission_type = %2 AND "
                  "pk_Department = %3 AND pk_Department_stream = %4 AND "
                  "year = %5 AND semester = %6 AND section = %7"};
    m_sqlTableSupported.setFilter(where.arg(programIndex)
                                      .arg(addmissionIndex)
                                      .arg(departmentIndex)
                                      .arg(streamIndex)
                                      .arg(year)
                                      .arg(semester)
                                      .arg(section));
    m_sqlTableSupported.select();
    return m_sqlTableSupported.rowCount() > 0;
}
