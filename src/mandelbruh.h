#include "pch.h"
#include <QImage>

long double map(long double value, long double in_min, long double in_max, long double out_min, long double out_max);

QImage renderMandelbruh(QSize size);

void scaleMandelbruh(QSize size, int x, int y, long double factor = 0.5);
