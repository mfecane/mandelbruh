#pragma once

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>

class MandelbruhView : public QQuickPaintedItem
{
Q_OBJECT

public:

    MandelbruhView(QQuickItem* parent = nullptr);
    void paint(QPainter* painter) override;

public slots:

    void updateImage();
    void zoomIn(int x, int y);
    void zoomOut(int x, int y);

private:

    long double zoom = 1;
    QImage m_image;
};

