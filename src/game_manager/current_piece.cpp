#include "current_piece.h"

GameManager::CurrentPiece *GameManager::CurrentPiece::_instance = nullptr;

GameManager::CurrentPiece::CurrentPiece() {
    this->_pieceLockedEvent = new Utils::Event();
}

GameManager::CurrentPiece *GameManager::CurrentPiece::getInstance() {
    if (CurrentPiece::_instance == nullptr) {
        CurrentPiece::_instance = new CurrentPiece();
    }

    return CurrentPiece::_instance;
}

bool GameManager::CurrentPiece::reset() {
    this->_pieceType = DataManager::PieceType::T;
    unsigned char *layout = DataManager::getPieceLayout(this->_pieceType, -1);
    DataManager::getLayoutBounds(layout, this->_startX, this->_startY, this->_endX, this->_endY);
    this->_rotation = DataManager::getDefaultRotationForPiece(this->_pieceType);

    delete[] layout;
    return true;
}

bool GameManager::CurrentPiece::rotateCounterClockwise() {
    return true;
}

bool GameManager::CurrentPiece::rotateClockwise() {
    return true;
}

void GameManager::CurrentPiece::performSoftDrop() {

}

void GameManager::CurrentPiece::performFall() {

}

void GameManager::CurrentPiece::addPieceLockedEventHandler(Utils::eventListener *callback) {
    this->_pieceLockedEvent->addListener(callback);
}

bool GameManager::CurrentPiece::removePieceLockedEventHandler(Utils::eventListener *callback) {
    return this->_pieceLockedEvent->removeListener(callback);
}