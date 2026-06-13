#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QTextCodec>
#include "area.h"

// Главное окно приложения: содержит холст (Area) и кнопку «Завершить».
class Window : public QWidget
{
  protected:
    QTextCodec *codec; // кодек для русификации заголовка и кнопки
    Area *area;        // область отображения рисунка
    QPushButton *btn;  // кнопка завершения работы

  public:
    Window();
};

#endif // WINDOW_H
