#include "play_area.h"

QFont UI::PlayArea::_pixelFont;

UI::PlayArea::PlayArea(QWidget *parent) : QWidget(parent) {
    this->setMinimumWidth(150);
    this->setMinimumHeight(250);
    this->setContentsMargins(0, 0, 0, 0);

    int id = QFontDatabase::addApplicationFont(":/assets/press_start_2P.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    PlayArea::_pixelFont = QFont(fontFamily);
    this->setFont(PlayArea::_pixelFont);

    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    // debug only!
    //this->setAttribute(Qt::WA_StyledBackground, true);
    //this->setStyleSheet("background-color: #424242;");

    QBoxLayout *horizontalBox = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);

    QBoxLayout *leftBarBox = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
    leftBarBox->setContentsMargins(0, 0, 0, 0);
    leftBarBox->setAlignment(Qt::AlignTop | Qt::AlignRight);
    horizontalBox->addLayout(leftBarBox, 1);
    this->createLeftBar(leftBarBox);

    this->_renderArea = new RenderArea(this);
    horizontalBox->addWidget(this->_renderArea, 1);

    QBoxLayout *rightBarBox = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
    rightBarBox->setContentsMargins(0, 0, 0, 0);
    rightBarBox->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    horizontalBox->addLayout(rightBarBox, 1);
    this->createRightBar(rightBarBox);

    DataManager::RuntimeData::addDataChangedCallback(std::bind(&UI::PlayArea::onDataChanged, this));
}

UI::PlayArea::~PlayArea() {}

void UI::PlayArea::onDataChanged() {
    this->setLevelNumber(DataManager::RuntimeData::getLevel());
    this->setScore(DataManager::RuntimeData::getScore());
    this->setLineCount(DataManager::RuntimeData::getClearedLines());
}


void UI::PlayArea::keyPressEvent(QKeyEvent *e) {
    if (e->isAutoRepeat()) {
        return;
    }

    if (e->key() == Qt::Key::Key_Escape) {
        GameManager::MainLoop::getInstance()->togglePause();
        UI::MainView::getInstance()->setPauseScreenVisibility(GameManager::MainLoop::getInstance()->isPaused());
        return;
    }

    switch (e->key()) {
    case Qt::Key::Key_Left:
        GameManager::CurrentPiece::getInstance()->setMoveLeftKeyState(true);
        break;
    case Qt::Key::Key_Right:
        GameManager::CurrentPiece::getInstance()->setMoveRightKeyState(true);
        break;
    case Qt::Key::Key_Down:
        GameManager::CurrentPiece::getInstance()->setSoftDropKeyState(true);
        break;
    case Qt::Key::Key_Z:
        GameManager::CurrentPiece::getInstance()->setRotateCounterClockwiseKeyState(true);
        break;
    case Qt::Key::Key_X:
        GameManager::CurrentPiece::getInstance()->setRotateClockwiseKeyState(true);
        break;
    }
}

void UI::PlayArea::keyReleaseEvent(QKeyEvent *e) {
    if (e->isAutoRepeat()) {
        return;
    }

    switch (e->key()) {
    case Qt::Key::Key_Left:
        GameManager::CurrentPiece::getInstance()->setMoveLeftKeyState(false);
        break;
    case Qt::Key::Key_Right:
        GameManager::CurrentPiece::getInstance()->setMoveRightKeyState(false);
        break;
    case Qt::Key::Key_Down:
        GameManager::CurrentPiece::getInstance()->setSoftDropKeyState(false);
        break;
    case Qt::Key::Key_Z:
        GameManager::CurrentPiece::getInstance()->setRotateCounterClockwiseKeyState(false);
        break;
    case Qt::Key::Key_X:
        GameManager::CurrentPiece::getInstance()->setRotateClockwiseKeyState(false);
        break;
    }
}

QFont UI::PlayArea::getDataPixelFont() {
    return PlayArea::_pixelFont;
}

void UI::PlayArea::setScore(int score) {
    std::string scoreString = std::to_string(score);
    if (scoreString.length() < 6)
    {
        scoreString.insert(0, 6 - scoreString.length(), '0');
    }

    scoreString.insert(0, "SCORE\n");
    this->_scoreLabel->setText(scoreString.c_str());
}

void UI::PlayArea::setLineCount(int lines) {
    std::string linesString = std::to_string(lines);
    if (linesString.length() < 3)
    {
        linesString = linesString.insert(0, 3 - linesString.length(), '0');
    }

    linesString.insert(0, "LINES\n");
    this->_linesLabel->setText(linesString.c_str());
}

void UI::PlayArea::setLevelNumber(int level) {
    std::string levelString = std::to_string(level);
    if (levelString.length() < 2)
    {
        levelString = levelString.insert(0, 2 - levelString.length(), '0');
    }

    levelString.insert(0, "LEVEL\n");
    this->_levelLabel->setText(levelString.c_str());
}

void UI::PlayArea::createLeftBar(QBoxLayout *column) {
    column->addSpacing(25);

    QLabel *gameModeLabel = new QLabel(this);
    gameModeLabel->setContentsMargins(0, 0, 0, 0);
    gameModeLabel->setFont(PlayArea::_pixelFont);
    gameModeLabel->setText("A-TYPE");
    gameModeLabel->setAlignment(Qt::AlignCenter);
    gameModeLabel->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    gameModeLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    gameModeLabel->setMaximumWidth(300);

    column->addWidget(gameModeLabel);
    column->addSpacing(45);
    column->addWidget(new StatisticsView(this));
}

void UI::PlayArea::createRightBar(QBoxLayout *column) {
    column->addSpacing(25);

    this->_scoreLabel = new QLabel(this);
    this->setScore(0);
    this->_scoreLabel->setContentsMargins(0, 0, 0, 0);
    this->_scoreLabel->setFont(PlayArea::_pixelFont);
    this->_scoreLabel->setAlignment(Qt::AlignCenter);
    this->_scoreLabel->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    this->_scoreLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    this->_scoreLabel->setMaximumWidth(300);

    column->addWidget(this->_scoreLabel);
    column->addSpacing(35);

    this->_linesLabel = new QLabel(this);
    this->setLineCount(0);
    this->_linesLabel->setContentsMargins(0, 0, 0, 0);
    this->_linesLabel->setFont(PlayArea::_pixelFont);
    this->_linesLabel->setAlignment(Qt::AlignCenter);
    this->_linesLabel->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    this->_linesLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    this->_linesLabel->setMaximumWidth(300);

    column->addWidget(this->_linesLabel);
    column->addSpacing(35);

    QWidget *nextPieceView = new NextPieceView(this);
    column->addWidget(nextPieceView);
    column->addSpacing(35);

    this->_levelLabel = new QLabel(this);
    this->setLevelNumber(0);
    this->_levelLabel->setContentsMargins(0, 0, 0, 0);
    this->_levelLabel->setFont(PlayArea::_pixelFont);
    this->_levelLabel->setAlignment(Qt::AlignCenter);
    this->_levelLabel->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    this->_levelLabel->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    this->_levelLabel->setMaximumWidth(240);

    column->addWidget(this->_levelLabel);
}

void UI::PlayArea::redraw() {}