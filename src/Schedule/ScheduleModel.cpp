#include "Schedule/ScheduleModel.h"
#include <QFile>
#include <QNetworkRequest>

Schedule_Model::Schedule_Model(QObject *parent)
    : QAbstractListModel(parent),
      m_db{QSqlDatabase::addDatabase("QSQLITE", "Schedule")},
      m_sqlTable(parent, m_db), m_currentRow(0), m_date(QDate::currentDate()) {
    m_db.setDatabaseName("schedule.db");
    m_db.open();
    m_sqlTable.setTable("ScheduleModel");
    setFilter();
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

    connect(this, &Schedule_Model::dateChanged, this,
            &Schedule_Model::setFilter);

    connect(this, &Schedule_Model::epochChanged, this,
            &Schedule_Model::downloadScheduleDB);

    QFile file("schedule.dbEPOCH");
    file.open(QFile::ReadOnly);
    m_epoch = file.readAll().toInt();
    file.close();

    connect(&m_nm, &QNetworkAccessManager::finished, [](QNetworkReply *reply) {
        reply->deleteLater(); //
    });

    setNetworkStatus(NetworkStatus::Waiting);
    fetch();
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

void Schedule_Model::setFilter() {
    beginResetModel();
    m_sqlTable.setFilter("weekday = strftime('%w','" +
                         m_date.toString("yyyy-MM-dd") + "')" +
                         " OR onceDate = " + m_date.toString("yyyy-MM-dd"));
    m_sqlTable.select();
    endResetModel();
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

void Schedule_Model::downloadScheduleDB() {

    QNetworkRequest nq{QUrl{"http://localhost/http/schedule.db"}};
    QNetworkReply *reply = m_nm.get(nq);

    connect(reply, &QNetworkReply::errorOccurred,
            [=](QNetworkReply::NetworkError error) {
                switch (error) {
                case QNetworkReply::HostNotFoundError:
                    setNetworkStatus(NetworkStatus::Waiting);
                    break;
                default:
                    setNetworkStatus(NetworkStatus::Error);
                }
            });

    setNetworkStatus(NetworkStatus::Downloading);
    connect(reply, &QNetworkReply::finished, this, [&, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            return;

        m_db.close();

        QFile file("schedule.db");
        file.open(QFile::WriteOnly);
        file.write(reply->readAll());
        file.close();

        m_db.open();

        setFilter();
        setNetworkStatus(NetworkStatus::Connected);
    });
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

Schedule_Model::NetworkStatus Schedule_Model::networkStatus() const {
    return m_networkStatus;
}

void Schedule_Model::setNetworkStatus(NetworkStatus message) {
    if (message == m_networkStatus)
        return;
    m_networkStatus = message;
    emit networkStatusChanged();
}

void Schedule_Model::fetch() {
    QNetworkReply *reply = m_nm.get(
        QNetworkRequest{QUrl{"http://localhost/http/schedule.dbEPOCH"}});

    connect(reply, &QNetworkReply::finished, [&, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            int epoch{reply->readAll().toInt()};
            setEpoch(epoch);
            setNetworkStatus(NetworkStatus::Connected);
        }
    });
    connect(reply, &QNetworkReply::socketStartedConnecting,
            [&]() { setNetworkStatus(NetworkStatus::Connecting); });
    connect(reply, &QNetworkReply::requestSent,
            [&]() { setNetworkStatus(NetworkStatus::Requesting); });
    connect(reply, &QNetworkReply::redirected,
            [&]() { setNetworkStatus(NetworkStatus::Redirected); });
    connect(reply, &QNetworkReply::metaDataChanged,
            [&]() { setNetworkStatus(NetworkStatus::Receiving); });
    connect(reply, &QNetworkReply::errorOccurred,
            [=](QNetworkReply::NetworkError error) {
                switch (error) {
                case QNetworkReply::HostNotFoundError:
                    setNetworkStatus(NetworkStatus::Waiting);
                    break;
                default:
                    setNetworkStatus(NetworkStatus::Error);
                }
            });
}

QString Schedule_Model::networkMessage(NetworkStatus id) const {
    static QHash<NetworkStatus, QString> message{
        {NetworkStatus::Connecting, "Connecting..."},
        {NetworkStatus::Requesting, "Requesting..."},
        {NetworkStatus::Redirected, "Redirected"},
        {NetworkStatus::Receiving, "Receiving..."},
        {NetworkStatus::Connected, "Connected"},
        {NetworkStatus::Downloading, "Downloading..."},
        {NetworkStatus::Waiting, "Waitting for network..."},
        {NetworkStatus::Error, "Network error"}};

    return message[id];
}

int Schedule_Model::epoch() const { return m_epoch; }

void Schedule_Model::setEpoch(int epoch) {
    if (epoch == m_epoch)
        return;
    m_epoch = epoch;
    emit epochChanged();

    QFile file("schedule.dbEPOCH");
    file.open(QFile::WriteOnly);
    file.write(QByteArray::number(m_epoch));
    file.close();
}
