#include "current_piece.h"

GameManager::CurrentPiece *GameManager::CurrentPiece::_instance = nullptr;

GameManager::CurrentPiece::CurrentPiece() {
    //this->reset();
    _pieceLockedEvent = new Utils::Event();
    _updateCallback = std::bind(&GameManager::CurrentPiece::onUpdate, this);
    MainLoop::getInstance()->addUpdateEventListener(_updateCallback);
}

DataManager::PieceData::PieceType GameManager::CurrentPiece::generateNextPiece() {
    int previousPiece = (int)_pieceType;
    int firstIteration = rand() % 8;

    if (firstIteration != previousPiece && firstIteration != 7) {
        return (PieceType)firstIteration;
    }

    return (PieceType)(rand() % 7);
}

GameManager::CurrentPiece *GameManager::CurrentPiece::getInstance() {
    if (CurrentPiece::_instance == nullptr) {
        CurrentPiece::_instance = new CurrentPiece();
    }

    return CurrentPiece::_instance;
}

void GameManager::CurrentPiece::onUpdate() {
    if (_areRemainingFrames > 0) {
        _areRemainingFrames--;
        return;
    }
    if (_lineClearRemainingFrames > 0) {
        _lineClearRemainingFrames--;
        return;
    }

    if (_isAreActive) {
        this->reset();
        _isAreActive = false;
        //prevent further soft drop
        _softDropFrame = -1;
        return;
    }

    if (_isMoveLeftKeyDown || _isMoveRightKeyDown) {
        _dasFrame--;
        if (_dasFrame == 0) {
            _dasFrame = 6;

            if (_isMoveLeftKeyDown) {
                this->moveToLeft();
            }
            else {
                this->moveToRight();
            }
        }
    }

    //NES IMPLEMENTATION UNCLEAR: does soft drop allow a normal gravity drop until the soft drop or not (implemented as no)?
    if (_isSoftDropKeyDown) {
        if (_softDropFrame == 0) {
            this->performFall();
            _softDropFrame = 1;
            return;
        }
        else if (_softDropFrame != -1) {
            _softDropFrame--;
            return;
        }
    }

    //we DO need to use a separate fall frame counter to correctly implement ARE
    _fallFrame++;

    if (_fallFrame % DataManager::RuntimeData::getGravitySpeed() == 0) {
        this->performFall();
    }
}

void GameManager::CurrentPiece::setLineClearDelay(int frameDelay) {
    _isAreActive = false;
    _areRemainingFrames = 0;

    _lineClearRemainingFrames = frameDelay;
}

void GameManager::CurrentPiece::getCurrentPieceData(PieceType &pieceType, int &rotation) {
    pieceType = _pieceType;
    rotation = _rotation;
}

void GameManager::CurrentPiece::getNextPieceData(PieceType &pieceType, int &rotation) {
    pieceType = _nextPieceType;
    rotation = _nextPieceRotation;
}

bool GameManager::CurrentPiece::reset() {
    if (_isFirstPiece) {
        _nextPieceType = this->generateNextPiece();
        _pieceLockedEvent->invoke();
        _nextPieceRotation = DataManager::PieceData::getDefaultRotationForPiece(_nextPieceType);
        _isFirstPiece = false;

        _lastUsedIndices[0] = 0;
        _lastUsedIndices[1] = 0;
        _lastUsedIndices[2] = 0;
        _lastUsedIndices[3] = 0;
        _fallFrame = 0;
    }

    _pieceType = _nextPieceType;
    _rotation = _nextPieceRotation;

    delete[] _cachedLayout;
    _cachedLayout = DataManager::PieceData::getPieceLayout(_pieceType, -1);
    DataManager::PieceData::getLayoutBounds(_cachedLayout, _startX, _startY, _endX, _endY);
    _xPos = 5;
    _yPos = 0;

    //check for game over
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            if (_cachedLayout[row * 5 + column] == 0) {
                continue;
            }

            int thisRow = _yPos - 2 + row, thisColumn = _xPos - 2 + column;
            if (GameManager::PlayField::getInstance()->getSquareType(thisRow, thisColumn)) {
                MainLoop::getInstance()->removeUpdateEventListener(_updateCallback);
                CurrentPiece::_instance = nullptr;
                _pieceLockedEvent->clearAllListeners();

                return false;
            }
        }
    }

    _nextPieceType = this->generateNextPiece();
    _nextPieceRotation = DataManager::PieceData::getDefaultRotationForPiece(_nextPieceType);

    _lastUsedIndices[0] = 0;
    _lastUsedIndices[1] = 0;
    _lastUsedIndices[2] = 0;
    _lastUsedIndices[3] = 0;

    this->redoLayout();
    return true;
}

bool GameManager::CurrentPiece::rotateCounterClockwise() {
    if (!this->canRotateCounterClockwise() || _isAreActive) {
        return false;
    }

    _rotation = (_rotation - 1) % 4;
    if (_rotation == -1) {
        _rotation = 3;
    }

    delete[] _cachedLayout;
    _cachedLayout = DataManager::PieceData::getPieceLayout(_pieceType, _rotation);
    DataManager::PieceData::getLayoutBounds(_cachedLayout, _startX, _startY, _endX, _endY);

    this->redoLayout();
    return true;
}

bool GameManager::CurrentPiece::rotateClockwise() {
    if (!this->canRotateClockwise() || _isAreActive) {
        return false;
    }

    _rotation = (_rotation + 1) % 4;
    if (_rotation == -1) {
        _rotation = 3;
    }

    delete[] _cachedLayout;
    _cachedLayout = DataManager::PieceData::getPieceLayout(_pieceType, _rotation);
    DataManager::PieceData::getLayoutBounds(_cachedLayout, _startX, _startY, _endX, _endY);

    this->redoLayout();
    return true;
}

void GameManager::CurrentPiece::performFall() {
    if (!this->canMoveDown()) {
        this->passControlToAre();
        _pieceLockedEvent->invoke();
        return;
    }

    _yPos++;
    this->redoLayout();
}

void GameManager::CurrentPiece::moveToLeft() {
    if (!this->canMoveLeft() || _isAreActive) {
        return;
    }

    _xPos--;
    this->redoLayout();
}

void GameManager::CurrentPiece::moveToRight() {
    if (!this->canMoveRight() || _isAreActive) {
        return;
    }

    _xPos++;
    this->redoLayout();
}

void GameManager::CurrentPiece::passControlToAre() {
    int frames = 10;
    int positionOfPieceBottom = _yPos + (_endY - 2);

    //NES IMPLEMENTATION: 10 for the 2 bottom rows, for each 4 rows an additional 2 frames are added
    if (positionOfPieceBottom >= 18) {
        frames = 10;
    }
    else if (positionOfPieceBottom >= 14) {
        frames = 12;
    }
    else if (positionOfPieceBottom >= 10) {
        frames = 14;
    }
    else if (positionOfPieceBottom >= 6) {
        frames = 16;
    }
    else if (positionOfPieceBottom >= 2) {
        frames = 18;
    }

    _areRemainingFrames = frames;
    _isAreActive = true;
}

void GameManager::CurrentPiece::addPieceLockedEventHandler(std::function<void()> callback) {
    _pieceLockedEvent->addListener(callback);
}

bool GameManager::CurrentPiece::removePieceLockedEventHandler(std::function<void()> callback) {
    return _pieceLockedEvent->removeListener(callback);
}


void GameManager::CurrentPiece::redoLayout() {
    //clear the last blocks (repaint)
    for (int i = 0; i < 4; i++) {
        GameManager::PlayField::getInstance()->setSquareType(_lastUsedIndices[i] / 10, _lastUsedIndices[i] % 10, 0);
    }

    int piece = 0;
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            if (_cachedLayout[row * 5 + column] != 0) {
                //determine the global indices (from the play field), not the local ones from the layout
                int thisRow = _yPos - 2 + row, thisColumn = _xPos - 2 + column;
                GameManager::PlayField::getInstance()->setSquareType(thisRow, thisColumn, _cachedLayout[row * 5 + column]);

                assert((piece < 4) && "More than 4 blocks found");
                _lastUsedIndices[piece] = thisRow * 10 + thisColumn;
                piece++;
            }
        }
    }
}

bool GameManager::CurrentPiece::canMoveDown() {
    if (_yPos + (_endY - 2) + 1 >= 20) {
        return false;
    }

    for (int column = 0; column < 5; column++) {
        //the lowest in the local layout
        int lowestRow = -1;
        for (int row = 0; row < 5; row++) {
            if (_cachedLayout[row * 5 + column] != 0) {
                lowestRow = row;
            }
        }

        if (lowestRow == -1) {
            continue;
        }

        int thisRow = _yPos - 2 + lowestRow, thisColumn = _xPos - 2 + column;
        if (GameManager::PlayField::getInstance()->getSquareType(thisRow + 1, thisColumn) != 0) {
            return false;
        }
    }

    return true;
}

bool GameManager::CurrentPiece::canMoveLeft() {
    //if the piece would exit the screen, return
    if (_xPos + (_startX - 2) - 1 < 0) {
        return false;
    }

    for (int row = 0; row < 5; row++) {
        int leftMostColumn = 5;
        for (int column = 4; column >= 0; column--) {
            if (_cachedLayout[row * 5 + column] != 0) {
                leftMostColumn = column;
            }
        }

        if (leftMostColumn == 5) {
            continue;
        }

        int thisRow = _yPos - 2 + row, thisColumn = _xPos - 2 + leftMostColumn;
        if (GameManager::PlayField::getInstance()->getSquareType(thisRow, thisColumn - 1) != 0) {
            return false;
        }
    }

    return true;
}

bool GameManager::CurrentPiece::canMoveRight() {
    //if the piece would exit the screen, return
    if (_xPos + (_endX - 2) + 1 >= 10) {
        return false;
    }

    for (int row = 0; row < 5; row++) {
        int rightMostColumn = -1;
        for (int column = 0; column < 5; column++) {
            if (_cachedLayout[row * 5 + column] != 0) {
                rightMostColumn = column;
            }
        }

        if (rightMostColumn == -1) {
            continue;
        }

        int thisRow = _yPos - 2 + row, thisColumn = _xPos - 2 + rightMostColumn;
        if (GameManager::PlayField::getInstance()->getSquareType(thisRow, thisColumn + 1) != 0) {
            return false;
        }
    }

    return true;
}

bool GameManager::CurrentPiece::canRotateClockwise() {
    unsigned char *projectedLayout = DataManager::PieceData::getPieceLayout(_pieceType, _rotation + 1);

    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            //if it is empty in the projected layout or if it's the same block as the one from the current rotation, skip it
            if (projectedLayout[row * 5 + column] == 0 ||
                (_cachedLayout[row * 5 + column] == projectedLayout[row * 5 + column]))
            {
                continue;
            }

            int thisRow = _yPos - 2 + row, thisColumn = _xPos - 2 + column;
            //if only one piece will occupy an already occupied block, then it's already impossible to rotate
            //if the piece would get outside the play field, you can not rotate
            if (GameManager::PlayField::getInstance()->getSquareType(thisRow, thisColumn) != 0 ||
                (thisRow >= 20 || thisColumn < 0 || thisColumn >= 10)) {
                delete[] projectedLayout;
                return false;
            }
        }
    }

    delete[] projectedLayout;
    return true;
}

bool GameManager::CurrentPiece::canRotateCounterClockwise() {
    unsigned char *projectedLayout = DataManager::PieceData::getPieceLayout(_pieceType, _rotation - 1);

    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            //if it's the same block as the one from the current rotation, skip it
            if (projectedLayout[row * 5 + column] == 0 ||
                (_cachedLayout[row * 5 + column] == projectedLayout[row * 5 + column]))
            {
                continue;
            }

            int thisRow = _yPos - 2 + row, thisColumn = _xPos - 2 + column;
            //if only one piece will occupy an already occupied block, then it's already impossible to rotate
            //if the piece would get outside the play field, you can not rotate
            if (GameManager::PlayField::getInstance()->getSquareType(thisRow, thisColumn) != 0 ||
                (thisRow >= 20 || thisColumn < 0 || thisColumn >= 10)) {
                delete[] projectedLayout;
                return false;
            }
        }
    }

    delete[] projectedLayout;
    return true;
}