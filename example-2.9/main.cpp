#include "window.h"

// Точка входа: создаём приложение, главное окно и запускаем цикл обработки событий.
int main(int argc, char *argv[])
{
    QApplication appl(argc, argv);
    Window win;
    if (!win.isReady()) // отложенная инициализация не удалась (сообщение уже показано)
        return 1;
    win.show();
    return appl.exec();
}
