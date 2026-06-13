#ifndef AREA_H
#define AREA_H

#include "figura.h"

// ’олст Ч область рисовани€. —оздаЄт фигуры и таймер, обрабатывает событи€
// показа/скрыти€, таймера и перерисовки (см. рис. 2.14).
class Area : public QWidget
{
    int myTimer;  // идентификатор таймера
    float alpha;  // текущий угол поворота фигур

  public:
    explicit Area(QWidget *parent = nullptr);
    ~Area();

    MyLine *myline; // вращающа€с€ лини€
    MyRect *myrect; // вращающийс€ квадрат

  protected:
    // ѕереопределЄнные обработчики событий:
    void paintEvent(QPaintEvent *event) override; // рисование фигур
    void timerEvent(QTimerEvent *event) override; // шаг анимации по тику таймера
    void showEvent(QShowEvent *event) override;   // запуск таймера при показе окна
    void hideEvent(QHideEvent *event) override;   // остановка таймера при скрытии окна
};

#endif // AREA_H
