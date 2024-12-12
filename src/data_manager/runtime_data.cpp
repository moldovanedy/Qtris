#include "runtime_data.h"

unsigned int _level = 0, _score = 0, _clearedLines = 0;
int _linesToNextLevel = 10;
Utils::Event *_dataChangedEvent = new Utils::Event();

unsigned int DataManager::RuntimeData::getLevel() {
    return _level;
}

unsigned int DataManager::RuntimeData::getScore() {
    return _score;
}

unsigned int DataManager::RuntimeData::getClearedLines() {
    return _clearedLines;
}

unsigned int DataManager::RuntimeData::getGravitySpeed() {
    switch (_level)
    {
    case 0:
        return 48;
    case 1:
        return 43;
    case 2:
        return 38;
    case 3:
        return 33;
    case 4:
        return 28;
    case 5:
        return 23;
    case 6:
        return 18;
    case 7:
        return 13;
    case 8:
        return 8;
    case 9:
        return 6;
    case 10:
    case 11:
    case 12:
        return 5;
    case 13:
    case 14:
    case 15:
        return 4;
    case 16:
    case 17:
    case 18:
        return 3;
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
        return 2;
    default:
    case 29:
        return 1;
    }
}

void DataManager::RuntimeData::addClearedLines(unsigned int lines) {
    if (lines > 4) {
        lines = 4;
    }

    //NES IMPLEMENTATION: the score will take the level that will be after these lines are cleared, 
    //so we increment the level before the calculation to mimic that behavior
    _linesToNextLevel -= lines;
    if (_linesToNextLevel <= 0) {
        _level++;
        _linesToNextLevel += 10;
    }

    switch (lines) {
    default:
    case 0:
        break;
    case 1:
        _score += 40 * (_level + 1);
        break;
    case 2:
        _score += 100 * (_level + 1);
        break;
    case 3:
        _score += 300 * (_level + 1);
        break;
    case 4:
        _score += 1200 * (_level + 1);
        break;
    }

    _clearedLines += lines;
    _dataChangedEvent->invoke();
}

void DataManager::RuntimeData::addSoftDropScore(unsigned int score) {
    if (score > 20) {
        score = 20;
    }

    _score += score;
    _dataChangedEvent->invoke();
}

void DataManager::RuntimeData::addDataChangedCallback(std::function<void()> callback) {
    _dataChangedEvent->addListener(callback);
}

void DataManager::RuntimeData::removeDataChangedCallback(std::function<void()> callback) {
    _dataChangedEvent->removeListener(callback);
}