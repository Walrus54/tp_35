#ifndef WIN_H
#define WIN_H

// В Qt5 классы виджетов вынесены из QtGui в отдельный модуль QtWidgets,
// поэтому подключаем именно его (в методичке для Qt4 был <QtGui>).
#include <QtWidgets>
// Класс QTextCodec, используемый для русификации интерфейса, — в модуле QtCore.
#include <QTextCodec>

// Класс главного окна приложения «Возведение числа в квадрат».
class Win : public QWidget
{
    Q_OBJECT // макрос Qt: обеспечивает корректную работу сигналов и слотов (обрабатывается MOC)

  protected:
    QTextCodec *codec;        // кодек Windows-1251 для русификации надписей
    QFrame *frame;            // рамка вокруг блока ввода/вывода
    QLabel *inputLabel;       // метка «Введите число:»
    QLineEdit *inputEdit;     // строчный редактор ввода исходного числа
    QLabel *outputLabel;      // метка «Результат:»
    QLineEdit *outputEdit;    // строчный редактор вывода результата
    QPushButton *nextButton;  // кнопка «Следующее» (новый ввод)
    QPushButton *exitButton;  // кнопка «Выход»

  public:
    explicit Win(QWidget *parent = nullptr); // конструктор: создаёт и компонует виджеты

  public slots:
    // Два собственных слота, добавленные к стандартным слотам QWidget:
    void begin(); // настройка интерфейса на ввод (см. рис. 2.6, а)
    void calc();  // вычисление квадрата и переключение интерфейса на вывод (рис. 2.6, б)
};

// Класс-валидатор строки ввода. Наследуется от QValidator.
// Объект этого класса назначается строчному редактору; при завершении ввода
// редактор автоматически вызывает validate(). Если метод вернул Acceptable,
// редактор генерирует сигналы editingFinished() и returnPressed().
class StrValidator : public QValidator
{
  public:
    explicit StrValidator(QObject *parent) : QValidator(parent) {}

    // Метод всегда принимает любую вводимую строку (возвращает Acceptable).
    // Корректность данных проверяется позже — в слоте Win::calc().
    State validate(QString &str, int &pos) const override
    {
        Q_UNUSED(str);
        Q_UNUSED(pos);
        return Acceptable;
    }
};

#endif // WIN_H
