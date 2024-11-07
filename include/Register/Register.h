#ifndef INCLUDE_REGISTER_REGISTER_H_
#define INCLUDE_REGISTER_REGISTER_H_

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStringListModel>

class Register : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList program READ program_type NOTIFY programChanged)
    Q_PROPERTY(
        QStringList addmission READ addmission_type NOTIFY addmissionChanged)
    Q_PROPERTY(QStringList department READ department NOTIFY departmentChanged)
    Q_PROPERTY(QStringList department_stream READ department_stream NOTIFY
                   department_streamChanged)
  public:
    explicit Register(QObject *parent = nullptr);

    void fill(QString tableName, QStringList &member);
    Q_INVOKABLE bool isSupported(int programIndex, int addmissionIndex,
                                 int year, int semester, int section,
                                 int departmentIndex, int streamIndex);
    QStringList program_type() const;
    QStringList addmission_type() const;
    QStringList department() const;
    QStringList department_stream() const;

  signals:
    void programChanged();
    void addmissionChanged();
    void departmentChanged();
    void department_streamChanged();

  private:
    QSqlDatabase m_db;
    QSqlTableModel m_sqlTable;
    QSqlTableModel m_sqlTableSupported;
    QStringList m_program_type;
    QStringList m_addmission_type;
    QStringList m_department;
    QStringList m_department_stream;
};

#endif // INCLUDE_REGISTER_REGISTER_H_
