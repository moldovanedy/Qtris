#include "play_area.h"

using namespace UI::Play;
using namespace GameManager;
using namespace DataManager;

PlayArea::PlayArea(QWidget *parent) : QWidget(parent) {
    this->setMinimumWidth(150);
    this->setMinimumHeight(250);
    this->setContentsMargins(0, 0, 0, 0);

    this->setFont(MainWindow::getInstance()->getAppFont());
    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    // debug only!
    // this->setAttribute(Qt::WA_StyledBackground, true);
    // this->setStyleSheet("background-color: #424242;");

    QBoxLayout *horizontalBox = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);

    QBoxLayout *leftBarBox = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
    leftBarBox->setContentsMargins(0, 0, 0, 0);
    leftBarBox->setAlignment(Qt::AlignTop | Qt::AlignRight);
    horizontalBox->addLayout(leftBarBox, 1);
    this->createLeftBar(leftBarBox);

    _renderArea = new RenderArea(this);
    horizontalBox->addWidget(_renderArea, 1);

    QBoxLayout *rightBarBox = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
    rightBarBox->setContentsMargins(0, 0, 0, 0);
    rightBarBox->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    horizontalBox->addLayout(rightBarBox, 1);
    this->createRightBar(rightBarBox);

    RuntimeData::addDataChangedCallback(std::bind(&PlayArea::onDataChanged, this));
}

PlayArea::~PlayArea() {}

void PlayArea::onDataChanged() {
    this->setLevelNumber(RuntimeData::getLevel());
    this->setScore(RuntimeData::getScore());
    this->setLineCount(RuntimeData::getClearedLines());
}


void PlayArea::keyPressEvent(QKeyEvent *e) {
    if (e->isAutoRepeat()) {
        return;
    }

    if (e->key() == Qt::Key::Key_Escape) {
        MainLoop::getInstance()->togglePause();
        MainWindow::getInstance()->setPauseScreenVisibility(MainLoop::getInstance()->isPaused());
        return;
    }

    switch (e->key()) {
    case Qt::Key::Key_Left:
        CurrentPiece::getInstance()->setMoveLeftKeyState(true);
        break;
    case Qt::Key::Key_Right:
        CurrentPiece::getInstance()->setMoveRightKeyState(true);
        break;
    case Qt::Key::Key_Down:
        CurrentPiece::getInstance()->setSoftDropKeyState(true);
        break;
    case Qt::Key::Key_Z:
        CurrentPiece::getInstance()->setRotateCounterClockwiseKeyState(true);
        break;
    case Qt::Key::Key_X:
        CurrentPiece::getInstance()->setRotateClockwiseKeyState(true);
        break;
    }
}

void PlayArea::keyReleaseEvent(QKeyEvent *e) {
    if (e->isAutoRepeat()) {
        return;
    }

    switch (e->key()) {
    case Qt::Key::Key_Left:
        CurrentPiece::getInstance()->setMoveLeftKeyState(false);
        break;
    case Qt::Key::Key_Right:
        CurrentPiece::getInstance()->setMoveRightKeyState(false);
        break;
    case Qt::Key::Key_Down:
        CurrentPiece::getInstance()->setSoftDropKeyState(false);
        break;
    case Qt::Key::Key_Z:
        CurrentPiece::getInstance()->setRotateCounterClockwiseKeyState(false);
        break;
    case Qt::Key::Key_X:
        CurrentPiece::getInstance()->setRotateClockwiseKeyState(false);
        break;
    }
}

void PlayArea::setScore(int score) {
    std::string scoreString = std::to_string(score);
    if (scoreString.length() < 6)
    {
        scoreString.insert(0, 6 - scoreString.length(), '0');
    }

    scoreString.insert(0, "SCORE\n");
    _scoreLabel->setText(scoreString.c_str());
}

void PlayArea::setLineCount(int lines) {
    std::string linesString = std::to_string(lines);
    if (linesString.length() < 3)
    {
        linesString = linesString.insert(0, 3 - linesString.length(), '0');
    }

    linesString.insert(0, "LINES\n");
    _linesLabel->setText(linesString.c_str());
}

void PlayArea::setLevelNumber(int level) {
    std::string levelString = std::to_string(level);
    if (levelString.length() < 2)
    {
        levelString = levelString.insert(0, 2 - levelString.length(), '0');
    }

    levelString.insert(0, "LEVEL\n");
    _levelLabel->setText(levelString.c_str());
}

void PlayArea::createLeftBar(QBoxLayout *column) {
    column->addSpacing(25);

    QLabel *gameModeLabel = new QLabel(this);
    gameModeLabel->setContentsMargins(0, 0, 0, 0);
    gameModeLabel->setFont(MainWindow::getInstance()->getAppFont());
    gameModeLabel->setText("A-TYPE");
    gameModeLabel->setAlignment(Qt::AlignCenter);
    gameModeLabel->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    gameModeLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    gameModeLabel->setMaximumWidth(300);

    column->addWidget(gameModeLabel);
    column->addSpacing(45);
    column->addWidget(new StatisticsView(this));
}

void PlayArea::createRightBar(QBoxLayout *column) {
    column->addSpacing(25);

    _scoreLabel = new QLabel(this);
    this->setScore(0);
    _scoreLabel->setContentsMargins(0, 0, 0, 0);
    _scoreLabel->setFont(MainWindow::getInstance()->getAppFont());
    _scoreLabel->setAlignment(Qt::AlignCenter);
    _scoreLabel->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    _scoreLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    _scoreLabel->setMaximumWidth(300);

    column->addWidget(_scoreLabel);
    column->addSpacing(35);

    _linesLabel = new QLabel(this);
    this->setLineCount(0);
    _linesLabel->setContentsMargins(0, 0, 0, 0);
    _linesLabel->setFont(MainWindow::getInstance()->getAppFont());
    _linesLabel->setAlignment(Qt::AlignCenter);
    _linesLabel->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    _linesLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    _linesLabel->setMaximumWidth(300);

    column->addWidget(_linesLabel);
    column->addSpacing(35);

    QWidget *nextPieceView = new NextPieceView(this);
    column->addWidget(nextPieceView);
    column->addSpacing(35);

    _levelLabel = new QLabel(this);
    this->setLevelNumber(DataManager::RuntimeData::getLevel());
    _levelLabel->setContentsMargins(0, 0, 0, 0);
    _levelLabel->setFont(MainWindow::getInstance()->getAppFont());
    _levelLabel->setAlignment(Qt::AlignCenter);
    _levelLabel->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    _levelLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    _levelLabel->setMaximumWidth(240);

    column->addWidget(_levelLabel);
}

void PlayArea::redraw() {}