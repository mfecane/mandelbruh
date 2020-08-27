#include "mandelbruhview.h"
#include "mandelbruh.h"

MandelbruhView::MandelbruhView(QQuickItem *parent):
    QQuickPaintedItem(parent)
{
    setImplicitWidth(600);
    setImplicitHeight(600);
    updateImage();
}

void MandelbruhView::paint(QPainter *painter)
{
    //m_image = QImage(600, 600, QImage::Format_RGB888);
    //m_image.fill(0);
    //updateImage();
    painter->drawImage(this->boundingRect(), m_image);
}

void MandelbruhView::updateImage()
{
    m_image = renderMandelbruh(QSize(this->width(), this->height()));
    //m_image = image;
    update();
}

void MandelbruhView::zoomIn(int x, int y)
{
    ::scaleMandelbruh(m_image.size(), x, y, 0.8);
    updateImage();
}

void MandelbruhView::zoomOut(int x, int y)
{
    ::scaleMandelbruh(m_image.size(), x, y, 1.2);
    updateImage();
}
