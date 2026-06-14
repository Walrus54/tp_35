#include "figura.h"
#include <math.h>

Figura::Figura(int X, int Y, int Halflen) : x(X), y(Y), halflen(Halflen) {}

// Виртуальный деструктор: обеспечивает корректное удаление наследников
// по указателю на базовый класс.
Figura::~Figura() {}

MyLine::MyLine(int x, int y, int halflen) : Figura(x, y, halflen) {}

MyRect::MyRect(int x, int y, int halflen) : Figura(x, y, halflen) {}

// Пересчёт смещения (dx, dy) для угла Alpha и вызов рисования.
// draw() — виртуальный, поэтому вызывается реализация конкретной фигуры (полиморфизм).
void Figura::move(float Alpha, QPainter *Painter)
{
    // Painter — сырой указатель, владения им метод не получает (рисует «чужим»
    // объектом). Если вызвать move() с nullptr, последующий draw() обратится
    // по нулевому адресу и приложение упадёт без объяснений. Поэтому проверяем
    // аргумент и сообщаем разработчику, что именно нарушено.
    Q_ASSERT_X(Painter != nullptr, "Figura::move",
               "передан нулевой QPainter — рисовать нечем");
    if (Painter == nullptr)
    {
        // В release-сборке Q_ASSERT_X отключён: не падаем, а выводим
        // диагностику в лог и пропускаем кадр.
        qWarning("Figura::move: получен нулевой указатель QPainter, "
                 "кадр пропущен");
        return;
    }

    dx = halflen * cos(Alpha);
    dy = halflen * sin(Alpha);
    draw(Painter);
}

// Линия: отрезок через центр, концы симметричны относительно (x, y).
void MyLine::draw(QPainter *Painter)
{
    Painter->drawLine(x + dx, y + dy, x - dx, y - dy);
}

// Квадрат: четыре стороны, построенные на повёрнутых векторах (dx, dy) и (dy, -dx).
void MyRect::draw(QPainter *Painter)
{
    Painter->drawLine(x + dx, y + dy, x + dy, y - dx);
    Painter->drawLine(x + dy, y - dx, x - dx, y - dy);
    Painter->drawLine(x - dx, y - dy, x - dy, y + dx);
    Painter->drawLine(x - dy, y + dx, x + dx, y + dy);
}
