#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QTextCodec>
#include "area.h"

// Главное окно приложения: содержит холст (Area) и кнопку «Завершить».
class Window : public QWidget
{
    // Поля закрыты (инкапсуляция): доступ нужен только методам окна.
  private:
    // codec — НЕвладеющий указатель (codecForName отдаёт разделяемый объект Qt).
    QTextCodec *codec;
    // Холст и кнопка — члены-значения (композиция): освобождаются автоматически
    // вместе с окном, без «new» и без риска утечки.
    Area area;        // область отображения рисунка
    QPushButton btn;  // кнопка завершения работы

  public:
    Window();
};

#endif // WINDOW_H
