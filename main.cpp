#include "win.h"

// Точка входа: создаём приложение, главное окно и запускаем цикл обработки событий.
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Win win(nullptr);
    win.show();
    return app.exec();
}
