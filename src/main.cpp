#include <iostream>
#include <QApplication>
#include <QFontDatabase>
#include <cstdlib>

#include "ui/play/main_view.h"
#include "game_manager/core_runtime.h"
#include "game_manager/current_piece.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication app(argc, argv);

    UI::MainWindow wnd;
    wnd.show();

    //initialization
    GameManager::CoreRuntime::getInstance();
    GameManager::CurrentPiece::getInstance();

    return app.exec();
}