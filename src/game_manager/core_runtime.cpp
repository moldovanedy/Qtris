#include "core_runtime.h"

using namespace GameManager;

CoreRuntime *CoreRuntime::_instance = nullptr;

CoreRuntime::CoreRuntime() {
    srand(time(NULL));
    MainLoop::getInstance()->addUpdateEventListener(std::bind(&CoreRuntime::onUpdate, this));

    CurrentPiece::getInstance()->addPieceLockedEventHandler(
        std::bind(&CoreRuntime::checkForLineClears, this));
}

CoreRuntime::~CoreRuntime() {
    _instance = nullptr;
}

CoreRuntime *CoreRuntime::getInstance() {
    if (CoreRuntime::_instance == nullptr) {
        _instance = new CoreRuntime();
    }

    return _instance;
}

void CoreRuntime::onUpdate() {
    if (_lineClearAnimationStepsLeft > 0) {
        if ((MainLoop::getInstance()->getFrameCounter() % 4) != 0) {
            return;
        }

        for (int i = 0; i < 4; i++) {
            PlayField::getInstance()->setSquareType(
                _rowsToClear[i],
                _lineClearAnimationStepsLeft,
                0);
            PlayField::getInstance()->setSquareType(
                _rowsToClear[i],
                10 - _lineClearAnimationStepsLeft,
                0);
        }

        _lineClearAnimationStepsLeft--;
    }
    //if there are no more animation frames but it still is in the animation it means it needs to update the score data.
    else if (_isInLineClearAnimation) {
        int linesCleared = 0;
        for (int i = 0; i < 4; i++) {
            if (_rowsToClear[i] != 0) {
                linesCleared++;
            }
        }

        int linesToGoDown = 0;
        for (int row = 19; row >= 0; row--) {
            bool isClear = false;
            for (int i = 0; i < 4; i++) {
                if (_rowsToClear[i] == row) {
                    linesToGoDown++;
                    isClear = true;
                    break;
                }
            }

            if (isClear || linesToGoDown == 0) {
                continue;
            }

            for (int column = 0; column < 10; column++) {
                int squareType = PlayField::getInstance()->getSquareType(row, column);
                PlayField::getInstance()->setSquareType(row + linesToGoDown, column, squareType);
            }
        }

        DataManager::RuntimeData::addClearedLines(linesCleared);
        _isInLineClearAnimation = false;
    }
}

void CoreRuntime::checkForLineClears() {
    PlayField *field = PlayField::getInstance();
    int linesCleared = 0;
    _rowsToClear[0] = 0;
    _rowsToClear[1] = 0;
    _rowsToClear[2] = 0;
    _rowsToClear[3] = 0;

    //get the lines to clear data
    for (int row = 19; row >= 0; row--) {
        bool mustBeCleared = true;
        for (int column = 0; column < 10; column++) {
            if (field->getSquareType(row, column) == 0) {
                mustBeCleared = false;
                break;
            }
        }

        if (mustBeCleared) {
            _rowsToClear[linesCleared] = row;
            linesCleared++;
        }
    }

    if (linesCleared > 0) {
        //NES IMPLEMENTATION: 5 animation steps that only run when the frame counter modulo 4 is 0, 
        //so the delay is between 17 and 20 frames
        CurrentPiece::getInstance()->setLineClearDelay(
            (MainLoop::getInstance()->getFrameCounter() % 4) + 17);

        _lineClearAnimationStepsLeft = 5;
        _isInLineClearAnimation = true;
    }

}