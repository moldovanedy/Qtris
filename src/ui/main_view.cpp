#include "main_view.h"
#include "menu_bar.h"

MainView::MainView(QWidget *parent) : QMainWindow(parent)
{
    this->setMinimumHeight(500);
    this->setMinimumWidth(715);

    this->_menuBar = new MenuBar(parent);
    this->setMenuBar(this->_menuBar);
    this->setCentralWidget(MainView::getStackPanel());
    this->showMaximized();
}

MainView::~MainView() {}

QStackedWidget *MainView::getStackPanel()
{
    this->mainStackPanel = new QStackedWidget();
    this->mainStackPanel->setStyleSheet("background-image: url(:/images/assets/bg.png); background-repeat: repeat-xy;");

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

            this->_mainContent = new QWidget();
            _mainContent->setStyleSheet("background-color: #00f; border: 2 solid #f00;");
            _mainContent->setMinimumWidth(150);
            _mainContent->setMinimumHeight(250);
            this->_horizontalContainer->addWidget(_mainContent);

            QWidget *rightSpacer = new QWidget();
            this->_horizontalContainer->addWidget(rightSpacer, 1);

            this->_verticalContainer->addLayout(this->_horizontalContainer);
        }

        QWidget *bottomSpacer = new QWidget();
        this->_verticalContainer->addWidget(bottomSpacer, 1);
    }

    this->mainStackPanel->addWidget(verticalWrapper);
    return this->mainStackPanel;
}

void MainView::resizeEvent(QResizeEvent *e)
{
    QSize windowSize = e->size();
    windowSize.setHeight(windowSize.height() - _menuBar->height());

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