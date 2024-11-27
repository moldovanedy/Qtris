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

void GameManager::MainLoop::addUpdateListener(Utils::eventListener *callback) {
    this->_updateEvent->addListener(callback);
}

bool GameManager::MainLoop::removeUpdateListener(Utils::eventListener *callback) {
    return this->_updateEvent->removeListener(callback);
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
}