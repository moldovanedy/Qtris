#include "main_loop.h"

GameManager::MainLoop *GameManager::MainLoop::_instance = nullptr;

GameManager::MainLoop::MainLoop(QWidget *parent) : QWidget(parent) {
    this->_updateEvent = new Utils::Event();
    this->_timer = new QTimer(this);
    this->connect(this->_timer, &QTimer::timeout, this, QOverload<>::of(&MainLoop::invokeEvent));
    this->_timer->start(16);
}

GameManager::MainLoop *GameManager::MainLoop::getInstance() {
    if (GameManager::MainLoop::_instance == nullptr) {
        GameManager::MainLoop::_instance = new MainLoop();
    }

    return GameManager::MainLoop::_instance;
}

void GameManager::MainLoop::addUpdateEventListener(std::function<void()> callback) {
    this->_updateEvent->addListener(callback);
}

bool GameManager::MainLoop::removeUpdateEventListener(std::function<void()> callback) {
    return this->_updateEvent->removeListener(callback);
}

/**
 * Returns a number that starts from 0 at the start of the application, indicating the frame number in this second (simulated time).
 */
int GameManager::MainLoop::getFrameCounter() {
    return _frameCounter;
}

void GameManager::MainLoop::invokeEvent() {
    if (_fastFrames >= 3) {
        _fastFrames = 0;
        this->_timer->setInterval(17);
    }
    else if (_fastFrames == 0) {
        this->_timer->setInterval(16);
    }

    _fastFrames++;
    this->_updateEvent->invoke();
    _frameCounter++;
}