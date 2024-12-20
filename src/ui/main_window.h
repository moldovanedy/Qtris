#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QResizeEvent>

#include <QDebug>
#include <QSizePolicy>

#include "play/play_area.h"
#include "menus/select_level.h"

#include "menu_bar.h"
#include "../game_manager/main_loop.h"

const QString QTRIS_DATA_AREA_STYLE = "background-color: #000; font-size: 22px; padding: 10px 20px; border: 2px solid #fff;";

namespace UI
{
    enum Scene {
        None = 0,
        StartScreen = 1,
        SelectLevel = 2,
        PlayScreen = 3,
        HighScores = 4
    };

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        static MainWindow *getInstance();

        QStackedWidget *getStackPanel();
        void setPauseScreenVisibility(bool shouldMakeVisible);

        int getMenuBarHeight();
        void setPauseScreenSize(int width, int height);

        Scene getCurrentScene();
        void setCurrentScene(Scene scene);

        QFont getAppFont();

    private:
        static MainWindow *_instance;

        MenuBar *_menuBar;
        QLabel *_pauseScreen;

        QWidget *_mainContent = nullptr;
        Scene _currentScene = Scene::None;

        QStackedWidget *_mainStackPanel;
        QFont _pixelFont;

        QString _currentFile;
    };
}

#endif // MAIN_VIEW_H
