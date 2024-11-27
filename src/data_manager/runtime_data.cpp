#include "runtime_data.h"

unsigned int _level = 0, _score = 0, _clearedLines = 0;

unsigned int DataManager::RuntimeData::getLevel() {
    return _level;
}

void DataManager::RuntimeData::setLevel(unsigned int level) {
    _level = level;
}

unsigned int DataManager::RuntimeData::getScore() {
    return _score;
}

void DataManager::RuntimeData::setScore(unsigned int score) {
    _score = score;
}

unsigned int DataManager::RuntimeData::getClearedLines() {
    return _clearedLines;
}

void DataManager::RuntimeData::setClearedLines(unsigned int lines) {
    _clearedLines = lines;
}