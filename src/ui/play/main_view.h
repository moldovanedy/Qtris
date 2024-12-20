#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QResizeEvent>

#include <QDebug>
#include <QSizePolicy>

#include "../menu_bar.h"
#include "play_area.h"
#include "../../game_manager/main_loop.h"

namespace UI::Play
{
    class MainView : public QWidget
    {
        Q_OBJECT

    public:
        explicit MainView(QWidget *parent = nullptr);
        ~MainView();

        static MainView *getInstance();

    protected:
        void resizeEvent(QResizeEvent *) override;

    private:
        static MainView *_instance;

        // PlayArea *_mainContent;
        QWidget *_mainContent;
        QString _currentFile;
    };
}

#endif // MAIN_VIEW_H
