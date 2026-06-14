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

// Конструктор: настраивает виджеты-члены, компонует окно (см. рис. 2.7),
// настраивает интерфейс на ввод и связывает сигналы со слотами.
// Все виджеты — члены-значения, поэтому создаются (и освобождаются) автоматически,
// здесь их остаётся только сконфигурировать. Родителем виджетам выступает this.
Win::Win(QWidget *parent)
    : QWidget(parent),
      codec(nullptr),
      frame(this),
      inputLabel(this),
      inputEdit(this),
      outputLabel(this),
      outputEdit(this),
      nextButton(this),
      exitButton(this)
{
    // Кодек для перевода строковых констант (в кодировке Windows-1251) в Unicode.
    // codecForName возвращает сырой указатель, который может быть nullptr, если
    // кодек не зарегистрирован в системе. Без проверки первый же codec->toUnicode()
    // разыменует ноль и приложение упадёт без объяснений.
    codec = QTextCodec::codecForName("Windows-1251");
    Q_ASSERT_X(codec != nullptr, "Win::Win", "кодек Windows-1251 недоступен в системе");
    if (codec == nullptr)
    {
        qWarning("Win::Win: кодек Windows-1251 недоступен, использую UTF-8");
        codec = QTextCodec::codecForName("UTF-8"); // запасной кодек — есть всегда
    }
    setWindowTitle(codec->toUnicode("Возведение в квадрат"));

    // --- настройка виджетов ---
    frame.setFrameShadow(QFrame::Raised); // приподнятая рамка
    frame.setFrameShape(QFrame::Panel);

    inputLabel.setText(codec->toUnicode("Введите число:"));

    // Назначаем редактору валидатор — без него не генерируется returnPressed().
    // validator — тоже член-значение, setValidator его владения не забирает.
    inputEdit.setValidator(&validator);

    outputLabel.setText(codec->toUnicode("Результат:"));

    nextButton.setText(codec->toUnicode("Следующее"));
    exitButton.setText(codec->toUnicode("Выход"));

    // --- компоновка согласно рис. 2.7 ---
    // Layout'ы создаются через new намеренно: setLayout/addLayout по дизайну Qt
    // забирают владение и сами удаляют их вместе с окном — это не утечка.
    // Левая колонка (внутри рамки): метки и поля ввода/вывода.
    QVBoxLayout *vLayout1 = new QVBoxLayout(&frame);
    vLayout1->addWidget(&inputLabel);
    vLayout1->addWidget(&inputEdit);
    vLayout1->addWidget(&outputLabel);
    vLayout1->addWidget(&outputEdit);
    vLayout1->addStretch(); // распорка прижимает виджеты вверх

    // Правая колонка: кнопки.
    QVBoxLayout *vLayout2 = new QVBoxLayout();
    vLayout2->addWidget(&nextButton);
    vLayout2->addWidget(&exitButton);
    vLayout2->addStretch();

    // Горизонтальная компоновка верхнего уровня: рамка + колонка кнопок.
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addWidget(&frame);
    hLayout->addLayout(vLayout2);

    begin(); // начальное состояние — режим ввода

    // --- связи «сигнал-слот» ---
    connect(&exitButton, &QPushButton::clicked, this, &Win::close);  // «Выход» -> закрыть окно
    connect(&nextButton, &QPushButton::clicked, this, &Win::begin);  // «Следующее» -> новый ввод
    connect(&inputEdit, &QLineEdit::returnPressed, this, &Win::calc);  // Enter в поле -> вычислить
}

// Настройка интерфейса на ввод: очищаем поле, прячем вывод,
// делаем кнопку «Следующее» неактивной, передаём фокус полю ввода.
void Win::begin()
{
    inputEdit.clear();
    nextButton.setEnabled(false);
    nextButton.setDefault(false);
    inputEdit.setEnabled(true);
    outputLabel.setVisible(false);
    outputEdit.setVisible(false);
    outputEdit.setEnabled(false);
    inputEdit.setFocus();
}

// Вычисление квадрата введённого числа и переключение интерфейса на вывод.
// Корректность ввода уже гарантирована StrValidator (returnPressed()
// генерируется только при состоянии Acceptable), поэтому проверка не нужна.
void Win::calc()
{
    float a = inputEdit.text().toFloat(); // ввод заведомо корректен
    QString str;
    str.setNum(a * a); // возведение в квадрат, число -> строка
    outputEdit.setText(str);
    // блокируем ввод и показываем результат
    inputEdit.setEnabled(false);
    outputLabel.setVisible(true);
    outputEdit.setVisible(true);
    nextButton.setDefault(true);
    nextButton.setEnabled(true);
    nextButton.setFocus();
}
