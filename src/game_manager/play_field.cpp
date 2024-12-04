#include "play_field.h"

GameManager::PlayField::PlayField() {
    // for (int i = 0; i < 50; i++) {
    //     this->_playField[i] = 0b01010101;
    // }
}

GameManager::PlayField *GameManager::PlayField::_instance = nullptr;

GameManager::PlayField *GameManager::PlayField::getInstance() {
    if (GameManager::PlayField::_instance == nullptr) {
        GameManager::PlayField::_instance = new GameManager::PlayField();
    }

    return GameManager::PlayField::_instance;
}

bool GameManager::PlayField::isSquareFree(int row, int column) {
    if (row < 0 || column < 0 || row >= NUMBER_OF_ROWS || column >= NUMBER_OF_COLUMNS) {
        return true;
    }

    int virtualIndex = row * 2 * NUMBER_OF_COLUMNS + column * 2;
    int indexOfByte = virtualIndex / 8;
    int indexInByte = virtualIndex % 8; //only for the first bit is enough

    return (this->_playField[indexOfByte] & (0b11 << indexInByte)) == 0;
}

int GameManager::PlayField::getSquareType(int row, int column) {
    if (row < 0 || column < 0 || row >= NUMBER_OF_ROWS || column >= NUMBER_OF_COLUMNS) {
        return 0;
    }

    int virtualIndex = row * 2 * NUMBER_OF_COLUMNS + column * 2;
    int indexOfByte = virtualIndex / 8;
    int indexInByte = virtualIndex % 8; //only for the first bit is enough

    int lsb = this->_playField[indexOfByte] & (1 << indexInByte);
    int msb = this->_playField[indexOfByte] & (1 << (indexInByte + 1));

    return (lsb != 0) | ((msb != 0) << 1);
}

void GameManager::PlayField::setSquareType(int row, int column, int squareType) {
    if (row < 0 || column < 0 || row >= NUMBER_OF_ROWS || column >= NUMBER_OF_COLUMNS) {
        return;
    }

    if (squareType < 0 || squareType > 3) {
        squareType = 0;
    }

    int virtualIndex = row * 2 * NUMBER_OF_COLUMNS + column * 2;
    int indexOfByte = virtualIndex / 8;
    int indexInByte = virtualIndex % 8; //only for the first bit is enough

    if ((squareType & 1) == 0) {
        this->_playField[indexOfByte] &= ~(1 << indexInByte);
    }
    else {
        this->_playField[indexOfByte] |= 1 << indexInByte;
    }

    if (((squareType >> 1) & 1) == 0) {
        this->_playField[indexOfByte] &= ~(1 << (indexInByte + 1));
    }
    else {
        this->_playField[indexOfByte] |= 1 << (indexInByte + 1);
    }
}