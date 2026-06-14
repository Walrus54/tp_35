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
// Конструктор: только берёт кодек и запускает отложенную инициализацию.
// Построение интерфейса вынесено в init() (ленивая инициализация).
Win::Win(QWidget *parent) : QWidget(parent)
{
    codec = QTextCodec::codecForName("UTF-8");
    QString errorMessage;
    initOk_ = init(&errorMessage);
    if (!initOk_)
        QMessageBox::critical(nullptr, QStringLiteral("Error"), errorMessage);
}

bool Win::isReady() const
{
    return initOk_;
}

// Отложенная инициализация: создаёт виджеты с проверкой каждого выделения,
// компонует (см. рис. 2.8) и связывает сигналы со слотами (см. рис. 2.9).
bool Win::init(QString *errorMessage)
{
    initOk_ = false;
    if (codec == nullptr)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: кодек UTF-8 недоступен.");
        return false;
    }
    this->setWindowTitle(codec->toUnicode("Счётчик"));

    label1 = new QLabel(codec->toUnicode("Счёт по 1"), this);
    if (!label1)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать метку 1.");
        return false;
    }
    label2 = new QLabel(codec->toUnicode("Счёт по 5"), this);
    if (!label2)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать метку 2.");
        return false;
    }
    edit1 = new Counter("0", this); // счётчик одиночных нажатий
    if (!edit1)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать счётчик 1.");
        return false;
    }
    edit2 = new Counter("0", this); // счётчик серий по пять
    if (!edit2)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать счётчик 2.");
        return false;
    }
    calcbutton = new QPushButton("+1", this);
    if (!calcbutton)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать кнопку «+1».");
        return false;
    }
    exitbutton = new QPushButton(codec->toUnicode("Выход"), this);
    if (!exitbutton)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать кнопку «Выход».");
        return false;
    }

    // Компоновка: три горизонтальных ряда (метки, счётчики, кнопки),
    // собранные в общую вертикальную колонку. Layout'ы передаются во владение Qt.
    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(label1);
    layout1->addWidget(label2);

    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addWidget(edit1);
    layout2->addWidget(edit2);

    QHBoxLayout *layout3 = new QHBoxLayout();
    layout3->addWidget(calcbutton);
    layout3->addWidget(exitbutton);

    QVBoxLayout *layout4 = new QVBoxLayout(this);
    layout4->addLayout(layout1);
    layout4->addLayout(layout2);
    layout4->addLayout(layout3);

    // Нажатие «+1» -> увеличить первый счётчик.
    connect(calcbutton, &QPushButton::clicked, edit1, &Counter::add_one);
    // Каждый пятый «тик» первого счётчика -> увеличить второй счётчик.
    connect(edit1, &Counter::tick_signal, edit2, &Counter::add_one);
    // «Выход» -> закрыть окно.
    connect(exitbutton, &QPushButton::clicked, this, &Win::close);

    initOk_ = true;
    return true;
}
