#ifndef AREA_H
#define AREA_H

#include <memory>
#include "figura.h"

// ’олст Ч область рисовани€. —оздаЄт фигуры и таймер, обрабатывает событи€
// показа/скрыти€, таймера и перерисовки (см. рис. 2.14).
class Area : public QWidget
{
    // ¬се пол€ закрыты (инкапсул€ци€): доступ нужен только методам самого холста.
    int myTimer;  // идентификатор таймера
    float alpha;  // текущий угол поворота фигур

    // ‘игуры Ч не QObject, поэтому не охвачены механизмом parent-child Qt.
    // ’раним их во владеющих умных указател€х (RAII): пам€ть освобождаетс€
    // автоматически в деструкторе Area, в том числе при исключении в конструкторе.
    std::unique_ptr<MyLine> myline; // вращающа€с€ лини€
    std::unique_ptr<MyRect> myrect; // вращающийс€ квадрат

  public:
    explicit Area(QWidget *parent = nullptr);
    // явный деструктор не нужен: unique_ptr сам освобождает фигуры.

  protected:
    // ѕереопределЄнные обработчики событий:
    void paintEvent(QPaintEvent *event) override; // рисование фигур
    void timerEvent(QTimerEvent *event) override; // шаг анимации по тику таймера
    void showEvent(QShowEvent *event) override;   // запуск таймера при показе окна
    void hideEvent(QHideEvent *event) override;   // остановка таймера при скрытии окна
};

#endif // AREA_H
