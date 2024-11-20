#include <iostream>
#include <QApplication>

#include "ui/main_view.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainView view;
    view.show();

    return app.exec();
}