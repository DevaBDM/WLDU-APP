#include "CopyFile.h"
#include <QDir>
#include <QFile>
#include <QObject>
#include <QPixmap>
#include <QQmlEngine>
#include <QSaveFile>
#include <QStandardPaths>

void CopyFile::save(const QUrl &url) {
    QString path{};
    if (url.isLocalFile())
        path = url.toLocalFile();
    else
        path = url.toString(QUrl::None);

    QFile file(path);
    file.open(QFile::ReadOnly);

    QString saveTo{
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)};
    QDir().mkpath(saveTo);
    QSaveFile image(saveTo + "/ProfilePic.png");
    image.open(QFile::WriteOnly);

    image.write(file.readAll());
    image.commit();
    file.close();
    emit finished(image.fileName());
}
