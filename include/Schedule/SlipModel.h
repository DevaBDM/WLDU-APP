#ifndef INCLUDE_SCHEDULE_SLIPMODEL_H_
#define INCLUDE_SCHEDULE_SLIPMODEL_H_

#include <QAbstractListModel>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QStringList>

class SlipModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int currentRow READ currentRow WRITE setCurrentRow NOTIFY currentRowChanged)
    Q_PROPERTY(QVariant title READ title NOTIFY titleChanged)
    Q_PROPERTY(QVariant shortNote READ shortNote NOTIFY shortNoteChanged)
    Q_PROPERTY(QVariant description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QStringList LetterGrade READ letterGrade NOTIFY letterGradeChanged)
    Q_PROPERTY(int GradeResult READ gradeResult NOTIFY gradeResultChanged)
  public:
    explicit SlipModel(QSqlDatabase &db, QAbstractListModel *parent = nullptr);
    enum Role {
        slipIDRole = Qt::UserRole + 1,
        codeRole,
        titleRole,
        ECTsRole,
        lectureHRole,
        tutorHRole,
        lab_practiceHRole,
        home_studyHRole,
        descriptionRole,
        gradeLetterRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;

    // properties
    void prepareModelDB();
    int currentRow() const;
    int gradeResult() const;
    void setCurrentRow(int row);
    void setGradeResult(int);

    QVariant title() const;
    QVariant shortNote() const;
    QVariant description() const;

  private:
    void connectCurrentRow();
    void fill(QString, QStringList &);
    QStringList letterGrade() const;

  public slots:
    void setGrade(int, int);

  signals:
    void currentRowChanged(int);
    void gradeResultChanged(int);
    void letterGradeChanged();

    void titleChanged();
    void shortNoteChanged();
    void descriptionChanged();

  private:
    QSqlDatabase &m_db;
    QSqlTableModel m_sqlTable;
    QSqlTableModel m_sqlTableGrade;
    int m_currentRow;
    QStringList m_LetterGrade_list;
    int m_gradeResult;
    QList<int> m_gradeChooseRow;
};

#endif // INCLUDE_SCHEDULE_SLIPMODEL_H_
