#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QResizeEvent>

#include <QDebug>
#include <QSizePolicy>

#include "menu_bar.h"
#include "play_area.h"
#include "../../game_manager/main_loop.h"

const QString QTRIS_DATA_AREA_STYLE = "background-color: #000; font-size: 22px; padding: 10px 20px; border: 2px solid #fff;";

namespace UI
{
    class MainView : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainView(QWidget *parent = nullptr);
        ~MainView();

        static MainView *getInstance();

        QStackedWidget *getStackPanel();
        void setPauseScreenVisibility(bool shouldMakeVisible);

    protected:
        void resizeEvent(QResizeEvent *) override;

    private:
        static MainView *_instance;

        MenuBar *_menuBar;
        // PlayArea *_mainContent;
        QWidget *_mainContent;
        QLabel *_pauseScreen;

        QStackedWidget *mainStackPanel;
        QBoxLayout *_verticalContainer;
        QBoxLayout *_horizontalContainer;
        QString _currentFile;
    };
}

#endif // MAIN_VIEW_H
