#include "main_window.h"

using namespace UI;

MainWindow *MainWindow::_instance = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    if (MainWindow::_instance == nullptr) {
        MainWindow::_instance = this;
    }

    int id = QFontDatabase::addApplicationFont(":/assets/press_start_2P.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    _pixelFont = QFont(fontFamily);

    this->setMinimumHeight(625);
    this->setMinimumWidth(900);

    _menuBar = new MenuBar(parent);
    this->setMenuBar(_menuBar);
    this->setCentralWidget(MainWindow::getStackPanel());
    this->showMaximized();
}

MainWindow::~MainWindow() {}

MainWindow *MainWindow::getInstance()
{
    if (MainWindow::_instance == nullptr) {
        MainWindow::_instance = new MainWindow();
    }

    return MainWindow::_instance;
}

QStackedWidget *MainWindow::getStackPanel()
{
    _mainStackPanel = new QStackedWidget();
    _mainStackPanel->setStyleSheet("background-image: url(:/assets/bg.png); background-repeat: repeat-xy;");

    this->setCurrentScene(Scene::SelectLevel);

    _mainStackPanel->addWidget(GameManager::MainLoop::getInstance());

    _pauseScreen = new QLabel("PAUSED");
    _pauseScreen->setFont(this->getAppFont());
    _pauseScreen->setAlignment(Qt::AlignCenter);
    _pauseScreen->setStyleSheet("font-size: 48px; background-color: #000; background-image: url();");
    _pauseScreen->setVisible(false);
    _mainStackPanel->addWidget(_pauseScreen);

    return _mainStackPanel;
}

void MainWindow::setPauseScreenVisibility(bool shouldMakeVisible)
{
    _pauseScreen->setVisible(shouldMakeVisible);
    _mainStackPanel->setCurrentWidget(shouldMakeVisible ? _pauseScreen : _mainContent);
}

int MainWindow::getMenuBarHeight() {
    return _menuBar->height();
}

void MainWindow::setPauseScreenSize(int width, int height) {
    _pauseScreen->setFixedSize(width, height);
}

Scene MainWindow::getCurrentScene() {
    return _currentScene;
}

void MainWindow::setCurrentScene(Scene scene, bool resetScene) {
    if (!resetScene && _currentScene == scene) {
        return;
    }

    if (_mainContent != nullptr) {
        delete _mainContent;
    }

    switch (scene) {
    case Scene::PlayScreen:
        GameManager::CurrentPiece::getInstance()->restart();
        _mainContent = Play::MainView::getInstance();
        break;
    case Scene::SelectLevel:
        GameManager::CurrentPiece::getInstance()->stop();
        _mainContent = new Menus::SelectLevel(this);
        break;
    default:
        break;
    }

    _mainStackPanel->insertWidget(0, _mainContent);
    _mainStackPanel->setCurrentWidget(_mainContent);
}

QFont MainWindow::getAppFont() {
    return _pixelFont;
}