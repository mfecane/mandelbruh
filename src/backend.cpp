#include "backend.h"

#include <QRandomGenerator>

BackEnd* BackEnd::m_instance = nullptr;


BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
    refreshImage();
}


QObject* BackEnd::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return (QObject*)getInstance();
}

void BackEnd::refreshImage()
{
    m_src = "image://imgprovider/" + QString::number(QRandomGenerator::global()->generate());
    emit srcChanged();
}


QString BackEnd::src()
{
    return m_src;
}

BackEnd *BackEnd::getInstance()
{
    if(!m_instance) {
        m_instance = new BackEnd();
    }
    return m_instance;
}


ImgProvider::ImgProvider():
    QQuickImageProvider(QQuickImageProvider::Image)
{ }


QImage ImgProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    (void)id;
    qDebug() << requestedSize;
    QImage image;
    if (size) *size = QSize(image.width(), image.height());
    return image;
}
