#include "mandelbruh.h"

#include <QGuiApplication>
#include <QScreen>
#include <QtWin>
#include <QPainter>

#include <math.h>

typedef struct _Coord {
    long double xmin = -2.0;
    long double xmax = 2.0;
    long double ymin = -2.0;
    long double ymax = 2.0;
} Coord;

Coord coord;

int MAX_ITERATIONS = 100;

long double map(long double x,
                long double in_min,
                long double in_max,
                long double out_min,
                long double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void scaleMandelbruh(QSize size, int x, int y, long double factor)
{
    //fix aspect ratio

    long double xsize = coord.xmax - coord.xmin;
    long double ysize = xsize / size.width() * size.height();
    long double ycenter = (coord.ymax + coord.ymin) / 2;

    coord.ymin = ycenter - ysize / 2;
    coord.ymax = ycenter + ysize / 2;

    // zoom point
    long double zx = map(x, 0, size.width(), coord.xmin, coord.xmax);
    long double zy = map(y, 0, size.height(), coord.ymin, coord.ymax);

    long double offsetx = zx * (1 - factor);
    long double offsety = zy * (1 - factor);

    coord.xmin = offsetx +  coord.xmin * factor;
    coord.xmax = offsetx +  coord.xmax * factor;
    coord.ymin = offsety +  coord.ymin * factor;
    coord.ymax = offsety +  coord.ymax * factor;
}

QImage renderMandelbruh(QSize size)
{
    QImage image(size, QImage::Format_RGB32);

    image.fill(0);

    for(int y = 0; y < image.height(); ++y)
        for(int x  = 0; x < image.width(); ++x) {

            long double a = map(x, 0, image.width(),  coord.xmin, coord.xmax);
            long double b = map(y, 0, image.height(), coord.ymin, coord.ymax);

            long double ai = a;
            long double bi = b;

            int n = 0;

            for(int i = 0; i < MAX_ITERATIONS; ++i) {

                long double a1 = a * a - b * b;
                long double b1 = 2 * a * b;

                a = a1 + ai;
                b = b1 + bi;

                if(a + b > 2) break;

                ++n;
            }

            byte bright = map(n, 0, MAX_ITERATIONS, 0, 255);
            if(n == MAX_ITERATIONS || bright  < 20) bright = 0;

            byte red = (byte)map(bright * bright, 0, 255 * 25, 0, 512) % 255;
            byte green = map(sqrt(bright), 0, sqrt(255), 0, 255);
            byte blue = bright;

            QColor col(red, green, blue);
            image.setPixel(x, y, col.rgb());
        }

    return image;
}

// steps :
// drag
// tweak algorithm and coloring

