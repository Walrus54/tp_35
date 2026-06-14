#include "win.h"

StrValidator::StrValidator(QObject *parent) : QValidator(parent) {}

// Посимвольная проверка вещественного числа (см. описание в win.h).
QValidator::State StrValidator::validate(QString &str, int &pos) const
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

// Конструктор: только берёт кодек и запускает отложенную инициализацию.
// Построение интерфейса вынесено в init() (ленивая инициализация): это позволяет
// проверить каждый шаг и при ошибке корректно сообщить пользователю, а не упасть.
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

// Отложенная инициализация: создаёт и компонует виджеты, связывает сигналы.
// На каждом шаге проверяет результат и при сбое заполняет errorMessage.
bool Win::init(QString *errorMessage)
{
    initOk_ = false;
    if (codec == nullptr)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: кодек UTF-8 недоступен.");
        return false;
    }
    setWindowTitle(codec->toUnicode("Возведение в квадрат"));

    // --- создание виджетов ---
    frame = new QFrame(this);
    if (!frame)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать рамку.");
        return false;
    }
    frame->setFrameShadow(QFrame::Raised); // приподнятая рамка
    frame->setFrameShape(QFrame::Panel);

    inputLabel = new QLabel(codec->toUnicode("Введите число:"), this);
    if (!inputLabel)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать метку ввода.");
        return false;
    }

    inputEdit = new QLineEdit("", this);
    if (!inputEdit)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать поле ввода.");
        return false;
    }

    // Назначаем редактору валидатор — без него не генерируется returnPressed().
    StrValidator *validator = new StrValidator(inputEdit);
    if (!validator)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать валидатор.");
        return false;
    }
    inputEdit->setValidator(validator);

    outputLabel = new QLabel(codec->toUnicode("Результат:"), this);
    if (!outputLabel)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать метку вывода.");
        return false;
    }

    outputEdit = new QLineEdit("", this);
    if (!outputEdit)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать поле вывода.");
        return false;
    }

    nextButton = new QPushButton(codec->toUnicode("Следующее"), this);
    if (!nextButton)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать кнопку «Следующее».");
        return false;
    }

    exitButton = new QPushButton(codec->toUnicode("Выход"), this);
    if (!exitButton)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Win: не удалось создать кнопку «Выход».");
        return false;
    }

    // --- компоновка согласно рис. 2.7 ---
    // Layout'ы создаются через new и передаются во владение Qt (виджету или
    // родительскому layout), который сам их удаляет вместе с окном.
    QVBoxLayout *vLayout1 = new QVBoxLayout(frame);
    vLayout1->addWidget(inputLabel);
    vLayout1->addWidget(inputEdit);
    vLayout1->addWidget(outputLabel);
    vLayout1->addWidget(outputEdit);
    vLayout1->addStretch(); // распорка прижимает виджеты вверх

    QVBoxLayout *vLayout2 = new QVBoxLayout();
    vLayout2->addWidget(nextButton);
    vLayout2->addWidget(exitButton);
    vLayout2->addStretch();

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addWidget(frame);
    hLayout->addLayout(vLayout2);

    begin(); // начальное состояние — режим ввода

    // --- связи «сигнал-слот» ---
    connect(exitButton, &QPushButton::clicked, this, &Win::close);  // «Выход» -> закрыть окно
    connect(nextButton, &QPushButton::clicked, this, &Win::begin);  // «Следующее» -> новый ввод
    connect(inputEdit, &QLineEdit::returnPressed, this, &Win::calc);  // Enter в поле -> вычислить

    initOk_ = true;
    return true;
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
// Корректность ввода уже гарантирована StrValidator (returnPressed()
// генерируется только при состоянии Acceptable), поэтому проверка не нужна.
void Win::calc()
{
    float a = inputEdit->text().toFloat(); // ввод заведомо корректен
    QString str;
    str.setNum(a * a); // возведение в квадрат, число -> строка
    outputEdit->setText(str);
    // блокируем ввод и показываем результат
    inputEdit->setEnabled(false);
    outputLabel->setVisible(true);
    outputEdit->setVisible(true);
    nextButton->setDefault(true);
    nextButton->setEnabled(true);
    nextButton->setFocus();
}
