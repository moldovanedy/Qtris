#include "main_loop.h"

using namespace GameManager;

MainLoop *MainLoop::_instance = nullptr;

MainLoop::MainLoop(QWidget *parent) : QWidget(parent) {
    _updateEvent = new Utils::Event();
    _timer = new QTimer(this);
    this->connect(_timer, &QTimer::timeout, this, QOverload<>::of(&MainLoop::invokeEvent));
    _timer->start(16);
}

MainLoop::~MainLoop() {
    _instance = nullptr;
    delete _updateEvent;
    delete _timer;
}

MainLoop *MainLoop::getInstance() {
    if (MainLoop::_instance == nullptr) {
        MainLoop::_instance = new MainLoop();
    }

    return MainLoop::_instance;
}

void MainLoop::addUpdateEventListener(std::function<void()> callback) {
    _updateEvent->addListener(callback);
}

bool MainLoop::removeUpdateEventListener(std::function<void()> callback) {
    return _updateEvent->removeListener(callback);
}

/**
 * Returns a number that starts from 0 at the start of the application, indicating the frame number in this second (simulated time).
 */
int MainLoop::getFrameCounter() {
    return _frameCounter;
}

void MainLoop::invokeEvent() {
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