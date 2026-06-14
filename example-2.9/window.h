#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QTextCodec>
#include "area.h"

// Главное окно приложения: содержит холст (Area) и кнопку «Завершить».
class Window : public QWidget
{
  public:
    Window(); // конструктор: запускает отложенную инициализацию

    // Проверка успешности отложенной инициализации (см. init()).
    bool isReady() const;

    // Поля закрыты (инкапсуляция): доступ нужен только методам окна.
  protected:
    // codec — НЕвладеющий указатель (codecForName отдаёт разделяемый объект Qt).
    QTextCodec *codec;
    // Холст и кнопка — дочерние объекты this (parent-child), Qt удаляет их сам.
    Area *area;       // область отображения рисунка
    QPushButton *btn; // кнопка завершения работы

  private:
    bool initOk_; // результат отложенной инициализации

    // Ленивая (двухфазная) инициализация интерфейса: вынесена из конструктора,
    // чтобы проверить каждый шаг и при сбое сообщить пользователю, а не упасть.
    bool init(QString *errorMessage);
};

#endif // WINDOW_H
