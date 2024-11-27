#include "main_view.h"

UI::MainView::MainView(QWidget *parent) : QMainWindow(parent)
{
    this->setMinimumHeight(625);
    this->setMinimumWidth(900);

    this->_menuBar = new MenuBar(parent);
    this->setMenuBar(this->_menuBar);
    this->setCentralWidget(MainView::getStackPanel());
    this->showMaximized();
}

UI::MainView::~MainView() {}

QStackedWidget *UI::MainView::getStackPanel()
{
    this->mainStackPanel = new QStackedWidget();
    this->mainStackPanel->setStyleSheet("background-image: url(:/assets/bg.png); background-repeat: repeat-xy;");

    QWidget *verticalWrapper = new QWidget();
    verticalWrapper->setStyleSheet("background-image: url();");
    this->_verticalContainer = new QBoxLayout(QBoxLayout::Direction::TopToBottom, verticalWrapper);
    this->_verticalContainer->setSpacing(0);
    this->_verticalContainer->setContentsMargins(0, 0, 0, 0);

    // add to verticalContainer
    {
        QWidget *topSpacer = new QWidget();
        this->_verticalContainer->addWidget(topSpacer, 1);

        this->_horizontalContainer = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
        this->_horizontalContainer->setSpacing(0);
        this->_horizontalContainer->setContentsMargins(0, 0, 0, 0);

        // add to horizontalContainer
        {
            QWidget *leftSpacer = new QWidget();
            this->_horizontalContainer->addWidget(leftSpacer, 1);

            this->_mainContent = new UI::PlayArea();
            this->_horizontalContainer->addWidget(this->_mainContent);

            QWidget *rightSpacer = new QWidget();
            this->_horizontalContainer->addWidget(rightSpacer, 1);

            this->_verticalContainer->addLayout(this->_horizontalContainer);
        }

        QWidget *bottomSpacer = new QWidget();
        this->_verticalContainer->addWidget(bottomSpacer, 1);
    }

    this->mainStackPanel->addWidget(verticalWrapper);

    this->mainStackPanel->addWidget(GameManager::MainLoop::getInstance());

    return this->mainStackPanel;
}

void UI::MainView::resizeEvent(QResizeEvent *e)
{
    QSize windowSize = e->size();
    windowSize.setHeight(windowSize.height() - this->_menuBar->height());

    float aspectRatio = windowSize.width() / (float)windowSize.height();

    // height matters
    if (aspectRatio > 1.5)
    {
        int height = windowSize.height();
        this->_mainContent->setFixedSize(height * 1.5, height);
    }
    // width matters
    else
    {
        int width = windowSize.width();
        this->_mainContent->setFixedSize(width, width * 0.66666667);
    }

    // this->mainContent->setFixedSize(300, 400);
}