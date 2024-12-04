#include "current_piece.h"

GameManager::CurrentPiece *GameManager::CurrentPiece::_instance = nullptr;

GameManager::CurrentPiece::CurrentPiece() {
    //this->reset();
    this->_pieceLockedEvent = new Utils::Event();
    this->_updateCallback = std::bind(&GameManager::CurrentPiece::onUpdate, this);
    MainLoop::getInstance()->addUpdateEventListener(this->_updateCallback);
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
    if (this->_areRemainingFrames > 0) {
        this->_areRemainingFrames--;
        return;
    }

    if (this->_isAreActive) {
        this->reset();
        _isAreActive = false;
        //prevent further soft drop
        this->_softDropFrame = -1;
    }

    if (this->_isMoveLeftKeyDown || this->_isMoveRightKeyDown) {
        this->_dasFrame--;
        if (this->_dasFrame == 0) {
            this->_dasFrame = 6;

            if (this->_isMoveLeftKeyDown) {
                this->moveToLeft();
            }
            else {
                this->moveToRight();
            }
        }
    }

    //NES IMPLEMENTATION UNCLEAR: does soft drop allow a normal gravity drop until the soft drop or not (implemented as no)?
    if (this->_isSoftDropKeyDown) {
        if (this->_softDropFrame == 0) {
            this->performFall();
            this->_softDropFrame = 1;
            return;
        }
        else {
            this->_softDropFrame--;
            return;
        }
    }

    //we DO need to use a separate fall frame counter to correctly implement ARE
    this->_fallFrame++;

    //TODO: get the frames for drop corresponding to each level
    if (this->_fallFrame % 15 == 0) {
        this->performFall();
    }
}

void GameManager::CurrentPiece::getCurrentPieceData(PieceType &pieceType, int &rotation) {
    pieceType = this->_pieceType;
    rotation = this->_rotation;
}

void GameManager::CurrentPiece::getNextPieceData(PieceType &pieceType, int &rotation) {
    pieceType = this->_nextPieceType;
    rotation = this->_nextPieceRotation;
}

bool GameManager::CurrentPiece::reset() {
    if (this->_isFirstPiece) {
        this->_nextPieceType = this->generateNextPiece();
        this->_nextPieceRotation = DataManager::PieceData::getDefaultRotationForPiece(this->_nextPieceType);
        this->_isFirstPiece = false;

        this->_lastUsedIndices[0] = 0;
        this->_lastUsedIndices[1] = 0;
        this->_lastUsedIndices[2] = 0;
        this->_lastUsedIndices[3] = 0;
        this->_fallFrame = 0;
    }

    this->_pieceType = this->_nextPieceType;
    this->_rotation = this->_nextPieceRotation;

    delete[] this->_cachedLayout;
    this->_cachedLayout = DataManager::PieceData::getPieceLayout(this->_pieceType, -1);
    DataManager::PieceData::getLayoutBounds(this->_cachedLayout, this->_startX, this->_startY, this->_endX, this->_endY);
    this->xPos = 5;
    this->yPos = 0;

    //check for game over
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            if (this->_cachedLayout[row * 5 + column] == 0) {
                continue;
            }

            int thisRow = this->yPos - 2 + row, thisColumn = this->xPos - 2 + column;
            if (GameManager::PlayField::getInstance()->getSquareType(thisRow, thisColumn)) {
                MainLoop::getInstance()->removeUpdateEventListener(this->_updateCallback);
                CurrentPiece::_instance = nullptr;
                this->_pieceLockedEvent->clearAllListeners();

                return false;
            }
        }
    }

    this->_nextPieceType = this->generateNextPiece();
    this->_nextPieceRotation = DataManager::PieceData::getDefaultRotationForPiece(this->_nextPieceType);

    this->_lastUsedIndices[0] = 0;
    this->_lastUsedIndices[1] = 0;
    this->_lastUsedIndices[2] = 0;
    this->_lastUsedIndices[3] = 0;

    this->redoLayout();
    return true;
}

bool GameManager::CurrentPiece::rotateCounterClockwise() {
    if (!this->canRotateCounterClockwise() || this->_isAreActive) {
        return false;
    }

    this->_rotation = (this->_rotation - 1) % 4;
    if (this->_rotation == -1) {
        this->_rotation = 3;
    }

    delete[] _cachedLayout;
    _cachedLayout = DataManager::PieceData::getPieceLayout(this->_pieceType, this->_rotation);
    DataManager::PieceData::getLayoutBounds(_cachedLayout, this->_startX, this->_startY, this->_endX, this->_endY);

    this->redoLayout();
    return true;
}

bool GameManager::CurrentPiece::rotateClockwise() {
    if (!this->canRotateClockwise() || this->_isAreActive) {
        return false;
    }

    this->_rotation = (this->_rotation + 1) % 4;
    if (this->_rotation == -1) {
        this->_rotation = 3;
    }

    delete[] _cachedLayout;
    _cachedLayout = DataManager::PieceData::getPieceLayout(this->_pieceType, this->_rotation);
    DataManager::PieceData::getLayoutBounds(_cachedLayout, this->_startX, this->_startY, this->_endX, this->_endY);

    this->redoLayout();
    return true;
}

void GameManager::CurrentPiece::performSoftDrop() {

}

void GameManager::CurrentPiece::performFall() {
    if (!this->canMoveDown()) {
        this->_pieceLockedEvent->invoke();
        this->passControlToAre();
        return;
    }

    this->yPos++;
    this->redoLayout();
}

void GameManager::CurrentPiece::moveToLeft() {
    if (!this->canMoveLeft() || this->_isAreActive) {
        return;
    }

    this->xPos--;
    this->redoLayout();
}

void GameManager::CurrentPiece::moveToRight() {
    if (!this->canMoveRight() || this->_isAreActive) {
        return;
    }

    this->xPos++;
    this->redoLayout();
}

void GameManager::CurrentPiece::passControlToAre() {
    int frames = 10;
    int positionOfPieceBottom = this->yPos + (this->_endY - 2);

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

    this->_areRemainingFrames = frames;
    this->_isAreActive = true;
}

void GameManager::CurrentPiece::addPieceLockedEventHandler(Utils::eventListener *callback) {
    this->_pieceLockedEvent->addListener(callback);
}

bool GameManager::CurrentPiece::removePieceLockedEventHandler(Utils::eventListener *callback) {
    return this->_pieceLockedEvent->removeListener(callback);
}


void GameManager::CurrentPiece::redoLayout() {
    //clear the last blocks (repaint)
    for (int i = 0; i < 4; i++) {
        GameManager::PlayField::getInstance()->setSquareType(this->_lastUsedIndices[i] / 10, this->_lastUsedIndices[i] % 10, 0);
    }

    int piece = 0;
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            if (_cachedLayout[row * 5 + column] != 0) {
                //determine the global indices (from the play field), not the local ones from the layout
                int thisRow = this->yPos - 2 + row, thisColumn = this->xPos - 2 + column;
                GameManager::PlayField::getInstance()->setSquareType(thisRow, thisColumn, _cachedLayout[row * 5 + column]);

                assert((piece < 4) && "More than 4 blocks found");
                this->_lastUsedIndices[piece] = thisRow * 10 + thisColumn;
                piece++;
            }
        }
    }
}

bool GameManager::CurrentPiece::canMoveDown() {
    if (this->yPos + (this->_endY - 2) + 1 >= 20) {
        return false;
    }

    for (int column = 0; column < 5; column++) {
        //the lowest in the local layout
        int lowestRow = -1;
        for (int row = 0; row < 5; row++) {
            if (this->_cachedLayout[row * 5 + column] != 0) {
                lowestRow = row;
            }
        }

        if (lowestRow == -1) {
            continue;
        }

        int thisRow = this->yPos - 2 + lowestRow, thisColumn = this->xPos - 2 + column;
        if (GameManager::PlayField::getInstance()->getSquareType(thisRow + 1, thisColumn) != 0) {
            return false;
        }
    }

    return true;
}

bool GameManager::CurrentPiece::canMoveLeft() {
    //if the piece would exit the screen, return
    if (this->xPos + (this->_startX - 2) - 1 < 0) {
        return false;
    }

    for (int row = 0; row < 5; row++) {
        int leftMostColumn = 5;
        for (int column = 4; column >= 0; column--) {
            if (this->_cachedLayout[row * 5 + column] != 0) {
                leftMostColumn = column;
            }
        }

        if (leftMostColumn == 5) {
            continue;
        }

        int thisRow = this->yPos - 2 + row, thisColumn = this->xPos - 2 + leftMostColumn;
        if (GameManager::PlayField::getInstance()->getSquareType(thisRow, thisColumn - 1) != 0) {
            return false;
        }
    }

    return true;
}

bool GameManager::CurrentPiece::canMoveRight() {
    //if the piece would exit the screen, return
    if (this->xPos + (this->_endX - 2) + 1 >= 10) {
        return false;
    }

    for (int row = 0; row < 5; row++) {
        int rightMostColumn = -1;
        for (int column = 0; column < 5; column++) {
            if (this->_cachedLayout[row * 5 + column] != 0) {
                rightMostColumn = column;
            }
        }

        if (rightMostColumn == -1) {
            continue;
        }

        int thisRow = this->yPos - 2 + row, thisColumn = this->xPos - 2 + rightMostColumn;
        if (GameManager::PlayField::getInstance()->getSquareType(thisRow, thisColumn + 1) != 0) {
            return false;
        }
    }

    return true;
}

bool GameManager::CurrentPiece::canRotateClockwise() {
    unsigned char *projectedLayout = DataManager::PieceData::getPieceLayout(this->_pieceType, this->_rotation + 1);

    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            //if it is empty in the projected layout or if it's the same block as the one from the current rotation, skip it
            if (projectedLayout[row * 5 + column] == 0 ||
                (this->_cachedLayout[row * 5 + column] == projectedLayout[row * 5 + column]))
            {
                continue;
            }

            int thisRow = this->yPos - 2 + row, thisColumn = this->xPos - 2 + column;
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
    unsigned char *projectedLayout = DataManager::PieceData::getPieceLayout(this->_pieceType, this->_rotation - 1);

    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            //if it's the same block as the one from the current rotation, skip it
            if (projectedLayout[row * 5 + column] == 0 ||
                (this->_cachedLayout[row * 5 + column] == projectedLayout[row * 5 + column]))
            {
                continue;
            }

            int thisRow = this->yPos - 2 + row, thisColumn = this->xPos - 2 + column;
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