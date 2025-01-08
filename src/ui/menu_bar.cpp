#include "menu_bar.h"
#include "main_window.h"
#include "../game_manager/current_piece.h"

using namespace UI;

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    this->addMenu(getGameMenu());
    this->addMenu(getHelpMenu());
}

MenuBar::~MenuBar() {}

QAction *MenuBar::getGameNewAction()
{
    QAction *action = new QAction("New", this);
    action->setShortcuts(QKeySequence::New);
    action->setStatusTip("Starts a new session without saving this session");
    QObject::connect(action, &QAction::triggered, this, &MenuBar::newGame);
    return action;
}

QAction *MenuBar::getGameQuitAction()
{
    QAction *action = new QAction("Quit", this);
    action->setShortcuts(QKeySequence::Quit);
    action->setStatusTip("Quits the game without saving this session");
    QObject::connect(action, &QAction::triggered, this, &MenuBar::quitApp);
    return action;
}

QAction *MenuBar::getHelpAboutAction()
{
    QAction *action = new QAction("About", this);
    action->setShortcuts(QKeySequence::HelpContents);
    QObject::connect(action, &QAction::triggered, this, &MenuBar::showAboutPopup);
    return action;
}

QMenu *MenuBar::getGameMenu()
{
    QMenu *menu = new QMenu("Game");
    menu->addAction(MenuBar::getGameNewAction());
    menu->addSeparator();
    menu->addAction(MenuBar::getGameQuitAction());
    return menu;
}

QMenu *MenuBar::getHelpMenu()
{
    QMenu *menu = new QMenu("Help");
    menu->addAction(MenuBar::getHelpAboutAction());
    return menu;
}

void MenuBar::newGame() {
    MainWindow::getInstance()->setCurrentScene(Scene::PlayScreen, true);
}

void MenuBar::quitApp() {
    MainWindow::getInstance()->close();
}

void MenuBar::showAboutPopup() {}