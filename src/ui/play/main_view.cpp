#include "main_view.h"

#include "../../game_manager/play_field.h"
#include "../../game_manager/core_runtime.h"

using namespace UI::Play;

MainView *MainView::_instance = nullptr;

MainView::MainView(QWidget *parent) : QWidget(parent) {
    if (MainView::_instance == nullptr) {
        MainView::_instance = this;
    }

    //initialization
    GameManager::CoreRuntime::getInstance();
    GameManager::CurrentPiece::getInstance();

    this->setStyleSheet("background-image: url();");
    QBoxLayout *verticalBoxContainer = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
    verticalBoxContainer->setSpacing(0);
    verticalBoxContainer->setContentsMargins(0, 0, 0, 0);

    // add to verticalContainer
    {
        QWidget *topSpacer = new QWidget();
        verticalBoxContainer->addWidget(topSpacer, 1);

        QBoxLayout *horizontalBoxContainer = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
        horizontalBoxContainer->setSpacing(0);
        horizontalBoxContainer->setContentsMargins(0, 0, 0, 0);

        // add to horizontalContainer
        {
            QWidget *leftSpacer = new QWidget();
            horizontalBoxContainer->addWidget(leftSpacer, 1);

            _mainContent = new PlayArea();
            horizontalBoxContainer->addWidget(_mainContent);

            QWidget *rightSpacer = new QWidget();
            horizontalBoxContainer->addWidget(rightSpacer, 1);

            verticalBoxContainer->addLayout(horizontalBoxContainer);
        }

        QWidget *bottomSpacer = new QWidget();
        verticalBoxContainer->addWidget(bottomSpacer, 1);
    }
}

MainView::~MainView() {
    MainView::_instance = nullptr;
    delete GameManager::PlayField::getInstance();
    delete GameManager::CoreRuntime::getInstance();
    delete GameManager::CurrentPiece::getInstance();
}

MainView *MainView::getInstance()
{
    if (MainView::_instance == nullptr) {
        MainView::_instance = new MainView();
    }

    return MainView::_instance;
}

void MainView::resizeEvent(QResizeEvent *e)
{
    QSize windowSize = e->size();
    windowSize.setHeight(windowSize.height() - MainWindow::getInstance()->getMenuBarHeight());

    float aspectRatio = windowSize.width() / (float)windowSize.height();
    MainWindow::getInstance()->setPauseScreenSize(windowSize.width(), windowSize.height());

    // height matters
    if (aspectRatio > 1.5)
    {
        int height = windowSize.height();
        _mainContent->setFixedSize(height * 1.5, height);
    }
    // width matters
    else
    {
        int width = windowSize.width();
        _mainContent->setFixedSize(width, width * 0.66666667);
    }
}