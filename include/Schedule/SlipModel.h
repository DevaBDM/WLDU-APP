#ifndef INCLUDE_SCHEDULE_SLIPMODEL_H_
#define INCLUDE_SCHEDULE_SLIPMODEL_H_

#include <QAbstractListModel>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>

class SlipModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int currentRow READ currentRow WRITE setCurrentRow NOTIFY currentRowChanged)
    Q_PROPERTY(QVariant title READ title NOTIFY titleChanged)
    Q_PROPERTY(QVariant shortNote READ shortNote NOTIFY shortNoteChanged)
    Q_PROPERTY(QVariant description READ description NOTIFY descriptionChanged)
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
        descriptionRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;

    // properties
    void prepareModelDB();
    int currentRow() const;
    void setCurrentRow(int row);

    QVariant title() const;
    QVariant shortNote() const;
    QVariant description() const;

  private:
    void connectCurrentRow();
  public slots:

  signals:
    void currentRowChanged(int);

    void titleChanged();
    void shortNoteChanged();
    void descriptionChanged();

  private:
    QSqlDatabase &m_db;
    QSqlTableModel m_sqlTable;
    int m_currentRow;
};

#endif // INCLUDE_SCHEDULE_SLIPMODEL_H_
