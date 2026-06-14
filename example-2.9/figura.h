#ifndef FIGURA_H
#define FIGURA_H

// QPainter и базовые типы рисования; в Qt5 — через модуль QtWidgets
// (в методичке для Qt4 был <QtGui>).
#include <QtWidgets>

// Абстрактный базовый класс фигуры (см. диаграмму классов, рис. 2.14).
// Демонстрирует «сложный полиморфизм»: невиртуальный move() базового класса
// вызывает виртуальный draw(), переопределённый в наследниках.
class Figura
{
  protected:
    int x, y;    // координаты геометрического центра фигуры
    int halflen; // половина характерного размера (длины линии / стороны квадрата)
    int dx, dy;  // текущее смещение, пересчитываемое при повороте
    int r;       // зарезервировано (как в методичке), не используется

    // Чисто виртуальный метод рисования — реализуется в наследниках.
    virtual void draw(QPainter *Painter) = 0;

  public:
    Figura(int X, int Y, int Halflen) : x(X), y(Y), halflen(Halflen) {}
    virtual ~Figura() {} // виртуальный деструктор — для корректного удаления по базовому указателю

    // Пересчитывает смещение под угол Alpha и перерисовывает фигуру.
    void move(float Alpha, QPainter *Painter);
};

// Линия, вращающаяся вокруг своего геометрического центра.
class MyLine : public Figura
{
  protected:
    void draw(QPainter *Painter) override;

  public:
    MyLine(int x, int y, int halflen) : Figura(x, y, halflen) {}
};

// Квадрат, вращающийся вокруг своего геометрического центра.
class MyRect : public Figura
{
  protected:
    void draw(QPainter *Painter) override;

  public:
    MyRect(int x, int y, int halflen) : Figura(x, y, halflen) {}
};

#endif // FIGURA_H
