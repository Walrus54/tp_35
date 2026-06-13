#ifndef WIN_H
#define WIN_H

// В Qt5 классы виджетов вынесены из QtGui в отдельный модуль QtWidgets
// (в методичке для Qt4 был <QtGui>).
#include <QtWidgets>
// QTextCodec для русификации находится в модуле QtCore.
#include <QTextCodec>

// Класс счётчика. Наследуется от QLineEdit (поле, отображающее число).
// Добавляет собственный сигнал tick_signal() и слот add_one() (см. рис. 2.10).
class Counter : public QLineEdit
{
    Q_OBJECT // макрос Qt: нужен для объявления собственных сигналов/слотов

  public:
    Counter(const QString &contents, QWidget *parent = nullptr)
        : QLineEdit(contents, parent)
    {
    }

  signals:
    // Сигнал «тик»: генерируется при достижении очередной серии из пяти нажатий.
    void tick_signal();

  public slots:
    // Слот «увеличить на единицу». Если текущее значение кратно пяти (и не ноль),
    // перед инкрементом генерирует tick_signal().
    void add_one()
    {
        QString str = text();
        int r = str.toInt();
        if (r != 0 && r % 5 == 0)
            emit tick_signal(); // достигнута серия из 5 -> оповещаем второй счётчик
        r++;
        str.setNum(r);
        setText(str);
    }
};

// Класс главного окна приложения «Счётчик».
class Win : public QWidget
{
    Q_OBJECT

  protected:
    QTextCodec *codec;            // кодек для русификации
    QLabel *label1, *label2;      // подписи «Счёт по 1» и «Счёт по 5»
    Counter *edit1, *edit2;       // два счётчика
    QPushButton *calcbutton;      // кнопка «+1»
    QPushButton *exitbutton;      // кнопка «Выход»

  public:
    explicit Win(QWidget *parent = nullptr);
};

#endif // WIN_H
