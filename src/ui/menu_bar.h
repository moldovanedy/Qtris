#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <QMenuBar>

namespace UI {
    class MenuBar : public QMenuBar
    {
        Q_OBJECT

    public:
        explicit MenuBar(QWidget *parent = nullptr);
        ~MenuBar();

        QAction *getGameNewAction();
        QAction *getGameQuitAction();

        QAction *getHelpAboutAction();

    private:
        QMenu *getGameMenu();
        QMenu *getHelpMenu();

        QString _currentFile;

    private slots:
        void newGame();
        void quitApp();

        void showAboutPopup();
    };
}
#endif // MENU_BAR_H