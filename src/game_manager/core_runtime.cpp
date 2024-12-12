#include "core_runtime.h"

GameManager::CoreRuntime *GameManager::CoreRuntime::_instance = nullptr;

GameManager::CoreRuntime::CoreRuntime() {
    srand(time(NULL));
    MainLoop::getInstance()->addUpdateEventListener(std::bind(&GameManager::CoreRuntime::onUpdate, this));

    GameManager::CurrentPiece::getInstance()->addPieceLockedEventHandler(
        std::bind(&GameManager::CoreRuntime::checkForLineClears, this));
}

GameManager::CoreRuntime *GameManager::CoreRuntime::getInstance() {
    if (GameManager::CoreRuntime::_instance == nullptr) {
        _instance = new GameManager::CoreRuntime();
    }

    return _instance;
}

void GameManager::CoreRuntime::onUpdate() {
    if (this->_lineClearAnimationStepsLeft > 0) {
        if ((MainLoop::getInstance()->getFrameCounter() % 4) != 0) {
            return;
        }

        for (int i = 0; i < 4; i++) {
            GameManager::PlayField::getInstance()->setSquareType(
                this->_rowsToClear[i],
                this->_lineClearAnimationStepsLeft,
                0);
            GameManager::PlayField::getInstance()->setSquareType(
                this->_rowsToClear[i],
                10 - this->_lineClearAnimationStepsLeft,
                0);
        }

        this->_lineClearAnimationStepsLeft--;
    }
    //if there are no more animation frames but it still is in the animation it means it needs to update the score data.
    else if (this->_isInLineClearAnimation) {
        int linesCleared = 0;
        for (int i = 0; i < 4; i++) {
            if (this->_rowsToClear[i] != 0) {
                linesCleared++;
            }
        }

        int linesToGoDown = 0;
        for (int row = 19; row >= 0; row--) {
            bool isClear = false;
            for (int i = 0; i < 4; i++) {
                if (this->_rowsToClear[i] == row) {
                    linesToGoDown++;
                    isClear = true;
                    break;
                }
            }

            if (isClear || linesToGoDown == 0) {
                continue;
            }

            for (int column = 0; column < 10; column++) {
                int squareType = GameManager::PlayField::getInstance()->getSquareType(row, column);
                GameManager::PlayField::getInstance()->setSquareType(row + linesToGoDown, column, squareType);
            }
        }

        DataManager::RuntimeData::addClearedLines(linesCleared);
        this->_isInLineClearAnimation = false;
    }
}

void GameManager::CoreRuntime::checkForLineClears() {
    GameManager::PlayField *field = GameManager::PlayField::getInstance();
    int linesCleared = 0;
    this->_rowsToClear[0] = 0;
    this->_rowsToClear[1] = 0;
    this->_rowsToClear[2] = 0;
    this->_rowsToClear[3] = 0;

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
        GameManager::CurrentPiece::getInstance()->setLineClearDelay(
            (MainLoop::getInstance()->getFrameCounter() % 4) + 17);

        this->_lineClearAnimationStepsLeft = 5;
        this->_isInLineClearAnimation = true;
    }

}