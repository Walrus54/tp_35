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

    // Поля закрыты (инкапсуляция): класс не наследуется, доступ к виджетам
    // нужен только его собственным методам. Все виджеты создаются с родителем
    // this, поэтому удаляются автоматически механизмом parent-child Qt (без утечек).
  private:
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
// Объект назначается строчному редактору; редактор вызывает validate() на
// каждое нажатие клавиши. Состояние Invalid отвергает символ (он не попадает
// в поле), Intermediate допускает незавершённый ввод, Acceptable разрешает
// генерацию editingFinished() и returnPressed().
class StrValidator : public QValidator
{
  public:
    explicit StrValidator(QObject *parent) : QValidator(parent) {}

    // Посимвольная проверка вещественного числа: необязательный знак «-»
    // первым символом, цифры и не более одной точки. Недопустимый символ
    // отбрасывается сразу при вводе, поэтому в Win::calc() проверка не нужна.
    State validate(QString &str, int &pos) const override
    {
        Q_UNUSED(pos);
        if (str.isEmpty())
            return Intermediate; // пустое поле — можно продолжать ввод

        bool hasDot = false;
        for (int i = 0; i < str.size(); ++i)
        {
            const QChar c = str.at(i);
            if (c == '-')
            {
                if (i != 0)
                    return Invalid; // минус допустим только первым символом
            }
            else if (c == '.')
            {
                if (hasDot)
                    return Invalid; // вторая точка недопустима
                hasDot = true;
            }
            else if (!c.isDigit())
                return Invalid; // любой другой символ запрещён
        }

        if (str == "-" || str == ".")
            return Intermediate; // пока незавершённое число

        return Acceptable;
    }
};

#endif // WIN_H
