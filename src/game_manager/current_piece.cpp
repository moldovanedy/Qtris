#include "current_piece.h"

using namespace GameManager;

CurrentPiece *CurrentPiece::_instance = nullptr;

CurrentPiece::CurrentPiece() {
    //this->reset();
    _pieceLockedEvent = new Utils::Event();
    _gameOverEvent = new Utils::Event();
    _updateCallback = std::bind(&CurrentPiece::onUpdate, this);
    MainLoop::getInstance()->addUpdateEventListener(_updateCallback);
}

DataManager::PieceData::PieceType CurrentPiece::generateNextPiece() {
    int previousPiece = (int)_pieceType;
    int firstIteration = rand() % 8;

    if (firstIteration != previousPiece && firstIteration != 7) {
        return (PieceType)firstIteration;
    }

    return (PieceType)(rand() % 7);
}

CurrentPiece::~CurrentPiece() {
    _instance = nullptr;

    _pieceLockedEvent->clearAllListeners();
    delete _pieceLockedEvent;
    _gameOverEvent->clearAllListeners();
    delete _gameOverEvent;
}

CurrentPiece *CurrentPiece::getInstance() {
    if (CurrentPiece::_instance == nullptr) {
        CurrentPiece::_instance = new CurrentPiece();
    }

    return CurrentPiece::_instance;
}

void CurrentPiece::onUpdate() {
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
            _softDropCount++;
            _softDropFrame = 1;
            return;
        }
        else if (_softDropFrame != -1) {
            _softDropFrame--;
            return;
        }
        //it is -1
        else {
            _softDropCount = 0;
        }
    }
    else {
        _softDropCount = 0;
    }

    //we DO need to use a separate fall frame counter to correctly implement ARE
    _fallFrame++;

    if (_fallFrame % DataManager::RuntimeData::getGravitySpeed() == 0) {
        this->performFall();
    }
}

void CurrentPiece::setLineClearDelay(int frameDelay) {
    _areRemainingFrames += frameDelay;

    _lineClearRemainingFrames = frameDelay;
}

void CurrentPiece::getCurrentPieceData(PieceType &pieceType, int &rotation) {
    pieceType = _pieceType;
    rotation = _rotation;
}

void CurrentPiece::getNextPieceData(PieceType &pieceType, int &rotation) {
    pieceType = _nextPieceType;
    rotation = _nextPieceRotation;
}

bool CurrentPiece::reset() {
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
            if (PlayField::getInstance()->getSquareType(thisRow, thisColumn)) {
                MainLoop::getInstance()->removeUpdateEventListener(_updateCallback);
                _pieceLockedEvent->clearAllListeners();
                _gameOverEvent->invoke();
                //CurrentPiece::_instance = nullptr;

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

bool CurrentPiece::rotateCounterClockwise() {
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

bool CurrentPiece::rotateClockwise() {
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

void CurrentPiece::performFall() {
    if (!this->canMoveDown()) {
        this->passControlToAre();
        //NES IMPLEMENTATION: a soft drop of 16 or above will restart from 10 (due to a bug), so it's 14, 15, 10, 11, 12 etc.
        DataManager::RuntimeData::addSoftDropScore(_softDropCount > 16 ? _softDropCount - 6 : _softDropCount);
        _pieceLockedEvent->invoke();
        return;
    }

    _yPos++;
    this->redoLayout();
}

void CurrentPiece::moveToLeft() {
    if (!this->canMoveLeft() || _isAreActive) {
        return;
    }

    _xPos--;
    this->redoLayout();
}

void CurrentPiece::moveToRight() {
    if (!this->canMoveRight() || _isAreActive) {
        return;
    }

    _xPos++;
    this->redoLayout();
}

void CurrentPiece::passControlToAre() {
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

    _areRemainingFrames += frames;
    _isAreActive = true;
}

void CurrentPiece::addPieceLockedEventHandler(std::function<void()> callback) {
    _pieceLockedEvent->addListener(callback);
}

bool CurrentPiece::removePieceLockedEventHandler(std::function<void()> callback) {
    return _pieceLockedEvent->removeListener(callback);
}

void CurrentPiece::addGameOverEventHandler(std::function<void()> callback) {
    _gameOverEvent->addListener(callback);
}

bool CurrentPiece::removeGameOverEventHandler(std::function<void()> callback) {
    return _gameOverEvent->removeListener(callback);
}


void CurrentPiece::redoLayout() {
    //clear the last blocks (repaint)
    for (int i = 0; i < 4; i++) {
        PlayField::getInstance()->setSquareType(_lastUsedIndices[i] / 10, _lastUsedIndices[i] % 10, 0);
    }

    int piece = 0;
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            if (_cachedLayout[row * 5 + column] != 0) {
                //determine the global indices (from the play field), not the local ones from the layout
                int thisRow = _yPos - 2 + row, thisColumn = _xPos - 2 + column;
                PlayField::getInstance()->setSquareType(thisRow, thisColumn, _cachedLayout[row * 5 + column]);

                assert((piece < 4) && "More than 4 blocks found");
                _lastUsedIndices[piece] = thisRow * 10 + thisColumn;
                piece++;
            }
        }
    }
}

bool CurrentPiece::canMoveDown() {
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
        if (PlayField::getInstance()->getSquareType(thisRow + 1, thisColumn) != 0) {
            return false;
        }
    }

    return true;
}

bool CurrentPiece::canMoveLeft() {
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
        if (PlayField::getInstance()->getSquareType(thisRow, thisColumn - 1) != 0) {
            return false;
        }
    }

    return true;
}

bool CurrentPiece::canMoveRight() {
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
        if (PlayField::getInstance()->getSquareType(thisRow, thisColumn + 1) != 0) {
            return false;
        }
    }

    return true;
}

bool CurrentPiece::canRotateClockwise() {
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
            if (PlayField::getInstance()->getSquareType(thisRow, thisColumn) != 0 ||
                (thisRow >= 20 || thisColumn < 0 || thisColumn >= 10)) {
                delete[] projectedLayout;
                return false;
            }
        }
    }

    delete[] projectedLayout;
    return true;
}

bool CurrentPiece::canRotateCounterClockwise() {
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
            if (PlayField::getInstance()->getSquareType(thisRow, thisColumn) != 0 ||
                (thisRow >= 20 || thisColumn < 0 || thisColumn >= 10)) {
                delete[] projectedLayout;
                return false;
            }
        }
    }

    delete[] projectedLayout;
    return true;
}