#pragma once

#include "pch.h"

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QColor>
#include <QTimer>
#include <QQuickImageProvider>

// ------------------------------------------

class BackEnd: public QObject
{
Q_OBJECT

    Q_PROPERTY(QString src READ src NOTIFY srcChanged)

public:

    QString src();

    static BackEnd *getInstance();
    static QObject* qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

public slots:

    void refreshImage();

signals:

    void srcChanged();

private:

    BackEnd(QObject* parent = nullptr);

    static BackEnd* m_instance;
    QString m_src;
}; // BackeEnd

// ------------------------------------------

class ImgProvider : public QQuickImageProvider
{

public:
    ImgProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
}; // ImgProvider
