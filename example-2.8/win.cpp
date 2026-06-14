#include "win.h"

// Конструктор окна: создаёт виджеты, компонует их (см. рис. 2.8)
// и устанавливает связи «сигнал-слот» (см. рис. 2.9).
Win::Win(QWidget *parent) : QWidget(parent)
{
    codec = QTextCodec::codecForName("Windows-1251");
    this->setWindowTitle(codec->toUnicode("Счётчик"));

    label1 = new QLabel(codec->toUnicode("Счёт по 1"), this);
    label2 = new QLabel(codec->toUnicode("Счёт по 5"), this);
    edit1 = new Counter("0", this); // счётчик одиночных нажатий
    edit2 = new Counter("0", this); // счётчик серий по пять
    calcbutton = new QPushButton("+1", this);
    exitbutton = new QPushButton(codec->toUnicode("Выход"), this);

    // Компоновка: три горизонтальных ряда (метки, счётчики, кнопки),
    // собранные в общую вертикальную колонку.
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
    connect(calcbutton, SIGNAL(clicked(bool)), edit1, SLOT(add_one()));
    // Каждый пятый «тик» первого счётчика -> увеличить второй счётчик.
    connect(edit1, SIGNAL(tick_signal()), edit2, SLOT(add_one()));
    // «Выход» -> закрыть окно.
    connect(exitbutton, SIGNAL(clicked(bool)), this, SLOT(close()));
}
