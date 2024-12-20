#include "main_loop.h"

GameManager::MainLoop *GameManager::MainLoop::_instance = nullptr;

GameManager::MainLoop::MainLoop(QWidget *parent) : QWidget(parent) {
    _updateEvent = new Utils::Event();
    _timer = new QTimer(this);
    this->connect(_timer, &QTimer::timeout, this, QOverload<>::of(&MainLoop::invokeEvent));
    _timer->start(16);
}

GameManager::MainLoop *GameManager::MainLoop::getInstance() {
    if (GameManager::MainLoop::_instance == nullptr) {
        GameManager::MainLoop::_instance = new MainLoop();
    }

    return GameManager::MainLoop::_instance;
}

void GameManager::MainLoop::addUpdateEventListener(std::function<void()> callback) {
    _updateEvent->addListener(callback);
}

bool GameManager::MainLoop::removeUpdateEventListener(std::function<void()> callback) {
    return _updateEvent->removeListener(callback);
}

/**
 * Returns a number that starts from 0 at the start of the application, indicating the frame number in this second (simulated time).
 */
int GameManager::MainLoop::getFrameCounter() {
    return _frameCounter;
}

void GameManager::MainLoop::invokeEvent() {
    if (_isPaused) {
        return;
    }

    if (_fastFrames >= 3) {
        _fastFrames = 0;
        _timer->setInterval(17);
    }
    else if (_fastFrames == 0) {
        _timer->setInterval(16);
    }

    _fastFrames++;
    _updateEvent->invoke();
    _frameCounter++;
}