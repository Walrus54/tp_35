#include "window.h"

// Конструктор: только берёт кодек и запускает отложенную инициализацию.
// Построение интерфейса вынесено в init() (ленивая инициализация).
Window::Window()
{
    codec = QTextCodec::codecForName("UTF-8");
    QString errorMessage;
    initOk_ = init(&errorMessage);
    if (!initOk_)
        QMessageBox::critical(nullptr, QStringLiteral("Error"), errorMessage);
}

bool Window::isReady() const
{
    return initOk_;
}

// Отложенная инициализация: создаёт холст и кнопку с проверкой каждого
// выделения, компонует их по вертикали и связывает кнопку со слотом закрытия.
bool Window::init(QString *errorMessage)
{
    initOk_ = false;
    if (codec == nullptr)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Window: кодек UTF-8 недоступен.");
        return false;
    }
    this->setWindowTitle(codec->toUnicode("Обработка событий"));

    area = new Area(this);
    if (!area)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Window: не удалось создать холст.");
        return false;
    }
    btn = new QPushButton(codec->toUnicode("Завершить"), this);
    if (!btn)
    {
        if (errorMessage)
            *errorMessage = QStringLiteral("Window: не удалось создать кнопку.");
        return false;
    }

    // Layout передаётся во владение this (конструктор с parent), Qt удалит его сам.
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(area);
    layout->addWidget(btn);

    connect(btn, &QPushButton::clicked, this, &Window::close);

    initOk_ = true;
    return true;
}
