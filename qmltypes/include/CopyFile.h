#ifndef INCLUDE_IMAGE_VIEWER_COPYDATA_H_
#define INCLUDE_IMAGE_VIEWER_COPYDATA_H_

#include <QObject>
#include <QQmlEngine>
#include <QUrl>

class CopyFile : public QObject {
    Q_OBJECT
    QML_ELEMENT
  public:
    CopyFile(QObject *parent = nullptr) : QObject(parent) {}
    Q_INVOKABLE void save(const QUrl &url);
  signals:
    void finished(QUrl path);
};

#endif // INCLUDE_IMAGE_VIEWER_COPYDATA_H_
