#include <iostream>
#include <QApplication>
#include <QFontDatabase>

#include "ui/play/main_view.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    UI::MainView view;
    view.show();

    return app.exec();
}