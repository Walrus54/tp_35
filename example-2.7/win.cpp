#include "win.h"

// Конструктор: создаёт все виджеты, компонует окно (см. рис. 2.7),
// настраивает интерфейс на ввод и связывает сигналы со слотами.
Win::Win(QWidget *parent) : QWidget(parent)
{
    // Кодек для перевода строковых констант (в кодировке Windows-1251) в Unicode.
    codec = QTextCodec::codecForName("Windows-1251");
    setWindowTitle(codec->toUnicode("Возведение в квадрат"));

    // --- создание виджетов ---
    frame = new QFrame(this);
    frame->setFrameShadow(QFrame::Raised); // приподнятая рамка
    frame->setFrameShape(QFrame::Panel);

    inputLabel = new QLabel(codec->toUnicode("Введите число:"), this);
    inputEdit = new QLineEdit("", this);

    // Назначаем редактору валидатор — без него не генерируется returnPressed().
    StrValidator *v = new StrValidator(inputEdit);
    inputEdit->setValidator(v);

    outputLabel = new QLabel(codec->toUnicode("Результат:"), this);
    outputEdit = new QLineEdit("", this);

    nextButton = new QPushButton(codec->toUnicode("Следующее"), this);
    exitButton = new QPushButton(codec->toUnicode("Выход"), this);

    // --- компоновка согласно рис. 2.7 ---
    // Левая колонка (внутри рамки): метки и поля ввода/вывода.
    QVBoxLayout *vLayout1 = new QVBoxLayout(frame);
    vLayout1->addWidget(inputLabel);
    vLayout1->addWidget(inputEdit);
    vLayout1->addWidget(outputLabel);
    vLayout1->addWidget(outputEdit);
    vLayout1->addStretch(); // распорка прижимает виджеты вверх

    // Правая колонка: кнопки.
    QVBoxLayout *vLayout2 = new QVBoxLayout();
    vLayout2->addWidget(nextButton);
    vLayout2->addWidget(exitButton);
    vLayout2->addStretch();

    // Горизонтальная компоновка верхнего уровня: рамка + колонка кнопок.
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addWidget(frame);
    hLayout->addLayout(vLayout2);

    begin(); // начальное состояние — режим ввода

    // --- связи «сигнал-слот» ---
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));  // «Выход» -> закрыть окно
    connect(nextButton, SIGNAL(clicked(bool)), this, SLOT(begin()));  // «Следующее» -> новый ввод
    connect(inputEdit, SIGNAL(returnPressed()), this, SLOT(calc()));  // Enter в поле -> вычислить
}

// Настройка интерфейса на ввод: очищаем поле, прячем вывод,
// делаем кнопку «Следующее» неактивной, передаём фокус полю ввода.
void Win::begin()
{
    inputEdit->clear();
    nextButton->setEnabled(false);
    nextButton->setDefault(false);
    inputEdit->setEnabled(true);
    outputLabel->setVisible(false);
    outputEdit->setVisible(false);
    outputEdit->setEnabled(false);
    inputEdit->setFocus();
}

// Вычисление квадрата введённого числа и переключение интерфейса на вывод.
void Win::calc()
{
    bool Ok = true;
    float r, a;
    QString str = inputEdit->text();
    a = str.toDouble(&Ok); // пытаемся преобразовать ввод в число
    if (Ok)
    {
        r = a * a;     // возведение в квадрат
        str.setNum(r); // число -> строка
        outputEdit->setText(str);
        // блокируем ввод и показываем результат
        inputEdit->setEnabled(false);
        outputLabel->setVisible(true);
        outputEdit->setVisible(true);
        nextButton->setDefault(true);
        nextButton->setEnabled(true);
        nextButton->setFocus();
    }
    else if (!str.isEmpty()) // непустой, но некорректный ввод -> сообщение об ошибке
    {
        QMessageBox msgBox(QMessageBox::Information,
                           codec->toUnicode("Возведение в квадрат."),
                           codec->toUnicode("Введено неверное значение."),
                           QMessageBox::Ok);
        msgBox.exec();
    }
}
