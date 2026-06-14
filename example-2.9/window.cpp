#include "window.h"

// Конструктор окна: холст и кнопка — члены-значения (уже созданы), здесь их
// остаётся настроить, скомпоновать по вертикали и связать со слотом закрытия.
Window::Window()
    : codec(nullptr),
      area(this),
      btn(this)
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

    btn.setText(codec->toUnicode("Завершить"));

    // Layout создаётся через new намеренно: Qt забирает владение и удалит его
    // вместе с окном — это не утечка.
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(&area);
    layout->addWidget(&btn);

    connect(&btn, &QPushButton::clicked, this, &Window::close);
}
