#include "window.h"

// Конструктор окна: создаёт холст и кнопку, компонует их по вертикали,
// связывает кнопку со слотом закрытия окна.
Window::Window()
{
    // codecForName возвращает сырой указатель, возможно nullptr (кодек не
    // зарегистрирован) — тогда первый же codec->toUnicode() уронит приложение.
    codec = QTextCodec::codecForName("Windows-1251");
    Q_ASSERT_X(codec != nullptr, "Window::Window", "кодек Windows-1251 недоступен в системе");
    if (codec == nullptr)
    {
        qWarning("Window::Window: кодек Windows-1251 недоступен, использую UTF-8");
        codec = QTextCodec::codecForName("UTF-8"); // запасной кодек — есть всегда
    }
    this->setWindowTitle(codec->toUnicode("Обработка событий"));

    area = new Area(this);
    btn = new QPushButton(codec->toUnicode("Завершить"), this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(area);
    layout->addWidget(btn);

    connect(btn, &QPushButton::clicked, this, &Window::close);
}
