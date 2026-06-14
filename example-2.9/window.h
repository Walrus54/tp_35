#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QTextCodec>
#include "area.h"

// Главное окно приложения: содержит холст (Area) и кнопку «Завершить».
class Window : public QWidget
{
    // Поля закрыты (инкапсуляция): класс не наследуется. Холст и кнопка создаются
    // с родителем this и освобождаются механизмом parent-child Qt (без утечек).
  private:
    QTextCodec *codec; // кодек для русификации заголовка и кнопки
    Area *area;        // область отображения рисунка
    QPushButton *btn;  // кнопка завершения работы

  public:
    Window();
};

#endif // WINDOW_H
