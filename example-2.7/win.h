#ifndef WIN_H
#define WIN_H

// В Qt5 классы виджетов вынесены из QtGui в отдельный модуль QtWidgets,
// поэтому подключаем именно его (в методичке для Qt4 был <QtGui>).
#include <QtWidgets>
// Класс QTextCodec, используемый для русификации интерфейса, — в модуле QtCore.
#include <QTextCodec>

// Класс-валидатор строки ввода. Наследуется от QValidator.
// Объект назначается строчному редактору; редактор вызывает validate() на
// каждое нажатие клавиши. Состояние Invalid отвергает символ (он не попадает
// в поле), Intermediate допускает незавершённый ввод, Acceptable разрешает
// генерацию editingFinished() и returnPressed().
class StrValidator : public QValidator
{
  public:
    explicit StrValidator(QObject *parent = nullptr);

    // Посимвольная проверка вещественного числа: необязательный знак «-»
    // первым символом, цифры и не более одной точки. Недопустимый символ
    // отбрасывается сразу при вводе, поэтому в Win::calc() проверка не нужна.
    State validate(QString &str, int &pos) const override;
};

// Класс главного окна приложения «Возведение числа в квадрат».
class Win : public QWidget
{
    Q_OBJECT // макрос Qt: обеспечивает корректную работу сигналов и слотов (обрабатывается MOC)

  public:
    explicit Win(QWidget *parent = nullptr); // конструктор: запускает отложенную инициализацию

    // Проверка успешности отложенной инициализации (см. init()).
    bool isReady() const;

  public slots:
    // Два собственных слота, добавленные к стандартным слотам QWidget:
    void begin(); // настройка интерфейса на ввод (см. рис. 2.6, а)
    void calc();  // вычисление квадрата и переключение интерфейса на вывод (рис. 2.6, б)

    // Поля закрыты (инкапсуляция): доступ к виджетам нужен только методам окна.
  protected:
    // codec — НЕвладеющий указатель: codecForName возвращает разделяемый объект Qt.
    QTextCodec *codec;
    // Виджеты — дочерние объекты this (parent-child), Qt удаляет их автоматически.
    QFrame *frame;            // рамка вокруг блока ввода/вывода
    QLabel *inputLabel;       // метка «Введите число:»
    QLineEdit *inputEdit;     // строчный редактор ввода исходного числа
    QLabel *outputLabel;      // метка «Результат:»
    QLineEdit *outputEdit;    // строчный редактор вывода результата
    QPushButton *nextButton;  // кнопка «Следующее» (новый ввод)
    QPushButton *exitButton;  // кнопка «Выход»

  private:
    bool initOk_; // результат отложенной инициализации

    // Ленивая (двухфазная) инициализация интерфейса: вынесена из конструктора,
    // чтобы проверить каждый шаг и при сбое сообщить пользователю, а не упасть.
    bool init(QString *errorMessage);
};

#endif // WIN_H
