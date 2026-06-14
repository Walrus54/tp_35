#include "win.h"

Counter::Counter(const QString &contents, QWidget *parent)
    : QLineEdit(contents, parent)
{
}

// Слот «увеличить на единицу»: при значении, кратном пяти (и не нуле),
// перед инкрементом генерирует tick_signal() для второго счётчика.
void Counter::add_one()
{
    QString str = text();
    int r = str.toInt();
    if (r != 0 && r % 5 == 0)
        emit tick_signal(); // достигнута серия из 5 -> оповещаем второй счётчик
    r++;
    str.setNum(r);
    setText(str);
}

// Конструктор окна: создаёт виджеты, компонует их (см. рис. 2.8)
// и устанавливает связи «сигнал-слот» (см. рис. 2.9).
// Конструктор: виджеты-члены уже созданы, здесь их остаётся настроить,
// скомпоновать (см. рис. 2.8) и связать сигналы со слотами (см. рис. 2.9).
// Счётчики инициализируются строкой "0", остальным виджетам родитель — this.
Win::Win(QWidget *parent)
    : QWidget(parent),
      codec(nullptr),
      label1(this),
      label2(this),
      edit1("0", this), // счётчик одиночных нажатий
      edit2("0", this), // счётчик серий по пять
      calcbutton("+1", this),
      exitbutton(this)
{
    // codecForName возвращает сырой указатель, возможно nullptr (кодек не
    // зарегистрирован) — тогда первый же codec->toUnicode() уронит приложение.
    codec = QTextCodec::codecForName("Windows-1251");
    Q_ASSERT_X(codec != nullptr, "Win::Win", "кодек Windows-1251 недоступен в системе");
    if (codec == nullptr)
    {
        qWarning("Win::Win: кодек Windows-1251 недоступен, использую UTF-8");
        codec = QTextCodec::codecForName("UTF-8"); // запасной кодек — есть всегда
    }
    this->setWindowTitle(codec->toUnicode("Счётчик"));

    label1.setText(codec->toUnicode("Счёт по 1"));
    label2.setText(codec->toUnicode("Счёт по 5"));
    exitbutton.setText(codec->toUnicode("Выход"));

    // Компоновка: три горизонтальных ряда (метки, счётчики, кнопки),
    // собранные в общую вертикальную колонку. Layout'ы создаются через new
    // намеренно: Qt забирает владение ими и сам удаляет вместе с окном.
    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(&label1);
    layout1->addWidget(&label2);

    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addWidget(&edit1);
    layout2->addWidget(&edit2);

    QHBoxLayout *layout3 = new QHBoxLayout();
    layout3->addWidget(&calcbutton);
    layout3->addWidget(&exitbutton);

    QVBoxLayout *layout4 = new QVBoxLayout(this);
    layout4->addLayout(layout1);
    layout4->addLayout(layout2);
    layout4->addLayout(layout3);

    // Нажатие «+1» -> увеличить первый счётчик.
    connect(&calcbutton, &QPushButton::clicked, &edit1, &Counter::add_one);
    // Каждый пятый «тик» первого счётчика -> увеличить второй счётчик.
    connect(&edit1, &Counter::tick_signal, &edit2, &Counter::add_one);
    // «Выход» -> закрыть окно.
    connect(&exitbutton, &QPushButton::clicked, this, &Win::close);
}
