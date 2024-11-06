#include "User/User.h"

User::User(QObject *parent)
    : QObject{parent}, m_db{QSqlDatabase::addDatabase("QSQLITE", "User")},
      m_sqlTableAccount{parent, m_db}, m_sqlTableStudent{parent, m_db} {
    m_db.setDatabaseName("Register/user.db");
    m_db.open();
    m_sqlTableAccount.setTable("Account");
    m_sqlTableAccount.select();
    m_sqlTableStudent.setTable("Student");
    m_sqlTableStudent.select();
}
bool User::registered() const { return m_sqlTableAccount.rowCount() > 0; }
QString User::userName() const {
    return m_sqlTableAccount.record(0).value("user_name").toString();
}
QUrl User::pp_location() const {
    return m_sqlTableAccount.record(0).value("pp_location").toString();
}
QString User::bio() const {
    return m_sqlTableAccount.record(0).value("Bio").toString();
}
// int level() const {
//     return m_sqlTableAccount.record(0).value("level").toInt();
// }

qint32 User::studentID() const {
    return m_sqlTableStudent.record(0).value("studentID").toInt();
}

QString User::program() const {
    return m_sqlTableStudent.record(0).value("Program").toString();
}
QString User::addmission() const {
    return m_sqlTableStudent.record(0).value("Addmission").toString();
}
int User::year() const {
    return m_sqlTableStudent.record(0).value("year").toInt();
}
int User::semester() const {
    return m_sqlTableStudent.record(0).value("semester").toInt();
}
int User::section() const {
    return m_sqlTableStudent.record(0).value("section").toInt();
}
QString User::department() const {
    return m_sqlTableStudent.record(0).value("Department").toString();
}
QString User::stream() const {
    return m_sqlTableStudent.record(0).value("Stream").toString();
}

void User::registerNew(QString userName, QString pp_location, QString Bio,
                       qint32 studentID, QString program, QString addmission,
                       int year, int semester, int section, QString department,
                       int departmentIndex, QString stream, int streamIndex) {
    QSqlRecord r{m_sqlTableAccount.record()};
    r.setValue("user_name", userName);
    r.setValue("pp_location", pp_location);
    r.setValue("Bio", Bio);
    m_sqlTableAccount.insertRecord(-1, r);
    m_sqlTableAccount.select();

    QSqlRecord rr{m_sqlTableStudent.record()};
    rr.setValue("studentID", studentID);
    rr.setValue("pk_Account", m_sqlTableAccount.rowCount());
    rr.setValue("Program", program);
    rr.setValue("addmission", addmission);
    rr.setValue("year", year);
    rr.setValue("semester", semester);
    rr.setValue("section", section);
    rr.setValue("pk_Department", departmentIndex);
    rr.setValue("Department", department);
    rr.setValue("pk_Stream", streamIndex);
    rr.setValue("Stream", stream);
    m_sqlTableStudent.insertRecord(-1, rr);
    m_sqlTableStudent.select();

    emit userNameChanged();
    emit pp_locationChanged();
    emit bioChanged();
    emit studentIDChanged();
    emit programChanged();
    emit addmissionChanged();
    emit yearChanged();
    emit semesterChanged();
    emit sectionChanged();
    emit departmentChanged();
    emit streamChanged();

    emit registeredChanged();
    emit registeredSuccessfully();
    emit profileInfoChanged();
}

QStringList User::profileInfo() {
    QStringList model;
    model.append(bio().isEmpty() ? "No Bio" : bio());
    model.append("WDU" + QString::number(studentID()));
    model.append(program() + "(" + addmission() + ")");
    model.append(QString::number(year()) + " Year " +
                 QString::number(semester()) + " semester" + ", section " +
                 QString::number(section()));
    model.append(department() + "(" + stream() + ")");

    return model;
}
