#include "area.h"

// Конструктор: фиксируем размер холста, создаём фигуры (через make_unique — RAII),
// обнуляем угол поворота.
Area::Area(QWidget *parent) : QWidget(parent)
{
    setFixedSize(QSize(300, 200));
    myline = std::make_unique<MyLine>(80, 100, 50);  // линия в левой части холста
    myrect = std::make_unique<MyRect>(220, 100, 50); // квадрат в правой части холста
    alpha = 0;
}

// Событие показа окна: запускаем таймер с интервалом 50 мс и запоминаем его id.
void Area::showEvent(QShowEvent *)
{
    myTimer = startTimer(50);
}

// Событие перерисовки: создаём QPainter и просим каждую фигуру нарисовать себя
// под текущим углом (квадрат вращается в обратную сторону и вдвое медленнее).
void Area::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    myline->move(alpha, &painter);
    myrect->move(alpha * (-0.5), &painter);
}

// Событие таймера: если сработал наш таймер — увеличиваем угол и планируем перерисовку.
void Area::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == myTimer) // если это наш таймер
    {
        alpha = alpha + 0.2;
        update(); // запланировать перерисовку (вызовет paintEvent)
    }
    else
        QWidget::timerEvent(event); // иначе — стандартная обработка
}

// Событие скрытия окна: останавливаем таймер.
void Area::hideEvent(QHideEvent *)
{
    killTimer(myTimer);
}
