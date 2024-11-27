#include "play_field.h"

GameManager::PlayField::PlayField() {}

GameManager::PlayField *GameManager::PlayField::_instance = nullptr;

GameManager::PlayField *GameManager::PlayField::getInstance() {
    if (GameManager::PlayField::_instance == nullptr) {
        GameManager::PlayField::_instance = new GameManager::PlayField();
    }

    return GameManager::PlayField::_instance;
}

bool GameManager::PlayField::isSquareFree(int row, int column) {
    if (row < 0 || column < 0 || row >= 20 || column >= 10) {
        return true;
    }

    int virtualIndex = row * 10 + column;
    int indexOfByte = virtualIndex / 8;
    int indexInByte = virtualIndex % 8;

    return (this->_playField[indexOfByte] & (1 << indexInByte)) == 0;
}