#include "play_field.h"

using namespace GameManager;

PlayField *PlayField::_instance = nullptr;

PlayField::PlayField() {
    for (int i = 0; i < 50; i++) {
        _playField[i] = 0;
    }
}

PlayField::~PlayField() {
    _instance = nullptr;
}

PlayField *PlayField::getInstance() {
    if (PlayField::_instance == nullptr) {
        PlayField::_instance = new PlayField();
    }

    return PlayField::_instance;
}

bool PlayField::isSquareFree(int row, int column) {
    if (row < 0 || column < 0 || row >= NUMBER_OF_ROWS || column >= NUMBER_OF_COLUMNS) {
        return true;
    }

    int virtualIndex = row * 2 * NUMBER_OF_COLUMNS + column * 2;
    int indexOfByte = virtualIndex / 8;
    int indexInByte = virtualIndex % 8; //only for the first bit is enough

    return (_playField[indexOfByte] & (0b11 << indexInByte)) == 0;
}

int PlayField::getSquareType(int row, int column) {
    if (row < 0 || column < 0 || row >= NUMBER_OF_ROWS || column >= NUMBER_OF_COLUMNS) {
        return 0;
    }

    int virtualIndex = row * 2 * NUMBER_OF_COLUMNS + column * 2;
    int indexOfByte = virtualIndex / 8;
    int indexInByte = virtualIndex % 8; //only for the first bit is enough

    int lsb = _playField[indexOfByte] & (1 << indexInByte);
    int msb = _playField[indexOfByte] & (1 << (indexInByte + 1));

    return (lsb != 0) | ((msb != 0) << 1);
}

void PlayField::setSquareType(int row, int column, int squareType) {
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
        _playField[indexOfByte] &= ~(1 << indexInByte);
    }
    else {
        _playField[indexOfByte] |= 1 << indexInByte;
    }

    if (((squareType >> 1) & 1) == 0) {
        _playField[indexOfByte] &= ~(1 << (indexInByte + 1));
    }
    else {
        _playField[indexOfByte] |= 1 << (indexInByte + 1);
    }
}