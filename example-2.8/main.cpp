#include "win.h"

// Точка входа: создаём приложение, главное окно и запускаем цикл обработки событий.
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Win win(nullptr);
    if (!win.isReady()) // отложенная инициализация не удалась (сообщение уже показано)
        return 1;
    win.show();
    return app.exec();
}
