#include "window.h"

// Точка входа: создаём приложение, главное окно и запускаем цикл обработки событий.
int main(int argc, char *argv[])
{
    QApplication appl(argc, argv);
    Window win;
    win.show();
    return appl.exec();
}
