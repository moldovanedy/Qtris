#include "core_runtime.h"

GameManager::CoreRuntime *GameManager::CoreRuntime::_instance = nullptr;

GameManager::CoreRuntime::CoreRuntime() {
    srand(time(NULL));
    this->_updateCallback = std::bind(&GameManager::CoreRuntime::onUpdate, this);
    MainLoop::getInstance()->addUpdateEventListener(this->_updateCallback);
}

GameManager::CoreRuntime *GameManager::CoreRuntime::getInstance() {
    if (GameManager::CoreRuntime::_instance == nullptr) {
        _instance = new GameManager::CoreRuntime();
    }

    return _instance;
}

void GameManager::CoreRuntime::onUpdate() {
    // int frameCount = MainLoop::getInstance()->getFrameCounter();
    // if (frameCount % 10 == 0) {
    //     GameManager::PlayField::getInstance()->setSquareType(rand() % 20, rand() % 10, 2);
    // }
}