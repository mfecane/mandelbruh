#pragma once

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

#include <QQuickWindow>

#include "pch.h"


class MandelbruhRenderer : public QObject, protected QOpenGLFunctions {

    Q_OBJECT

public:

    MandelbruhRenderer();

    void setT(qreal t);
    void setViewportSize(const QSize &size);
    void setWindow(QQuickWindow *window);

public slots:

    void init();
    void paint();

private:

    QSize m_viewportSize;
    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;
    qreal m_t;

};

class MandelbruhView2 : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    QML_ELEMENT

public:

    MandelbruhView2();

    qreal t() const;
    void setT(qreal t);

signals:
    void tChanged();

public slots:

    void sync();
    void cleanup();

private slots:

    void handleWindowChanged(QQuickWindow *win);

private:

    void releaseResources() override;

    MandelbruhRenderer *m_renderer;
    qreal m_t = 0;


};


