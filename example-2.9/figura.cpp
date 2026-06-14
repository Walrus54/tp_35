#include "figura.h"
#include <math.h>

// ѕересчЄт смещени€ (dx, dy) дл€ угла Alpha и вызов рисовани€.
// draw() Ч виртуальный, поэтому вызываетс€ реализаци€ конкретной фигуры (полиморфизм).
void Figura::move(float Alpha, QPainter *Painter)
{
    dx = halflen * cos(Alpha);
    dy = halflen * sin(Alpha);
    draw(Painter);
}

// Ћини€: отрезок через центр, концы симметричны относительно (x, y).
void MyLine::draw(QPainter *Painter)
{
    Painter->drawLine(x + dx, y + dy, x - dx, y - dy);
}

//  вадрат: четыре стороны, построенные на повЄрнутых векторах (dx, dy) и (dy, -dx).
void MyRect::draw(QPainter *Painter)
{
    Painter->drawLine(x + dx, y + dy, x + dy, y - dx);
    Painter->drawLine(x + dy, y - dx, x - dx, y - dy);
    Painter->drawLine(x - dx, y - dy, x - dy, y + dx);
    Painter->drawLine(x - dy, y + dx, x + dx, y + dy);
}
