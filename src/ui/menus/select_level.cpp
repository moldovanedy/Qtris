#include "select_level.h"

using namespace UI::Menus;

SelectLevel::SelectLevel(QWidget *parent) : QWidget(parent) {
    this->setStyleSheet("background-image: url();");
    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    QBoxLayout *verticalBoxContainer = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
    verticalBoxContainer->setSpacing(0);
    verticalBoxContainer->setContentsMargins(0, 0, 0, 0);

    // add to verticalContainer
    {
        QWidget *topSpacer = new QWidget();
        verticalBoxContainer->addWidget(topSpacer, 1);

        QBoxLayout *horizontalContainer = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
        horizontalContainer->setSpacing(0);
        horizontalContainer->setContentsMargins(0, 0, 0, 0);

        // add to horizontalContainer
        {
            QWidget *leftSpacer = new QWidget();
            horizontalContainer->addWidget(leftSpacer, 1);

            horizontalContainer->addLayout(this->getMainContent());

            QWidget *rightSpacer = new QWidget();
            horizontalContainer->addWidget(rightSpacer, 1);

            verticalBoxContainer->addLayout(horizontalContainer);
        }

        QWidget *bottomSpacer = new QWidget();
        verticalBoxContainer->addWidget(bottomSpacer, 1);
    }

    this->visuallySelectLevel();
}

SelectLevel::~SelectLevel() {}

void SelectLevel::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key::Key_Enter || e->key() == Qt::Key::Key_Return)
    {
        if (e->isAutoRepeat()) {
            return;
        }

        MainWindow::getInstance()->setCurrentScene(Scene::PlayScreen);
    }

    _lastSelectedLevel = _selectedLevel;

    switch (e->key())
    {
    case Qt::Key::Key_Left:
    {
        if (_selectedLevel == 0) {
            _selectedLevel = 19;
        }
        else {
            _selectedLevel--;
        }
        break;
    }
    case Qt::Key::Key_Up:
    {
        if (_selectedLevel < 5) {
            _selectedLevel = 15 + _selectedLevel;
        }
        else {
            _selectedLevel -= 5;
        }
        break;
    }
    case Qt::Key::Key_Down:
    {
        if (_selectedLevel > 14) {
            _selectedLevel = _selectedLevel - 15;
        }
        else {
            _selectedLevel += 5;
        }
        break;
    }
    case Qt::Key::Key_Right:
    {
        if (_selectedLevel >= 19) {
            _selectedLevel = 0;
        }
        else {
            _selectedLevel++;
        }
        break;
    }
    default:
        return;
    }

    this->visuallySelectLevel();
}

void SelectLevel::visuallySelectLevel() {
    _buttons[_lastSelectedLevel]->setStyleSheet("background-color: #000; border: 2px solid #fff; font-size: 24px;");
    _buttons[_selectedLevel]->setStyleSheet("background-color: #ff9800; border: 2px solid #fff; font-size: 24px;");
}

QBoxLayout *SelectLevel::getMainContent() {
    QBoxLayout *root = new QBoxLayout(QBoxLayout::Direction::TopToBottom);

    QLabel *label = new QLabel("Select level");
    label->setStyleSheet("font-size: 32px;");
    label->setFont(MainWindow::getInstance()->getAppFont());
    root->addWidget(label);

    root->addSpacing(30);

    QGridLayout *levelGrid = new QGridLayout();
    levelGrid->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < 20; i++) {
        QPushButton *levelButton = new QPushButton(this);
        levelButton->setFixedSize(QSize(60, 60));
        levelButton->setStyleSheet("background-color: #000; border: 2px solid #fff; font-size: 24px;");
        //NES IMPLEMENTATION: levels start at 0
        levelButton->setText(QString::fromStdString(std::to_string(i)));
        levelButton->setFont(MainWindow::getInstance()->getAppFont());
        levelButton->setCursor(Qt::PointingHandCursor);

        levelGrid->addWidget(levelButton, i / 5, i % 5);
        _buttons[i] = levelButton;
    }
    root->addLayout(levelGrid);

    return root;
}