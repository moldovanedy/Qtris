#include "runtime_data.h"

unsigned int _level = 0, _score = 0, _clearedLines = 0;
int _linesToNextLevel = 10;
bool _isLongLevel = false;
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

    //truncate the 1, 2 or 3 resulted form more than 1 line cleared
    unsigned int normalizedClrLines = _clearedLines / 10 * 10;

    //NES IMPLEMENTATION: after 2290 lines (when starting form level 19), the line count to the next level goes to 800 lines
    //because of a bug, then it happens at intervals of 2900 lines
    //TODO: implement correct behavior depending on the start level (it's unclear how, but at least for level 0 it happens at 2190 lines)
    _clearedLines += lines;
    if (!_isLongLevel && _clearedLines >= 2290 && (normalizedClrLines - 2290) % 2900 == 0) {
        _isLongLevel = true;
    }
    else if (_isLongLevel && _linesToNextLevel - lines <= 0) {
        _isLongLevel = false;
    }

    //NES IMPLEMENTATION: the score will take the level that will be after these lines are cleared, 
    //so we increment the level before the calculation to mimic that behavior
    _linesToNextLevel -= lines;
    if (_linesToNextLevel <= 0) {
        _level++;
        _linesToNextLevel += _isLongLevel ? 810 : 10;
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

    _dataChangedEvent->invoke();
}

void DataManager::RuntimeData::addSoftDropScore(unsigned int score) {
    if (score > 20) {
        score = 20;
    }

    _score += score;
    _dataChangedEvent->invoke();
}

void DataManager::RuntimeData::setStartLevel(unsigned int level) {
    if (level > __INT32_MAX__) {
        level = 0;
    }
    _level = std::min((int)level, 19);

    //NES IMPLEMENTATION: lines to the next level does not progress by increments of 10 as intended because of a bug in logic
    //therefore, implement the actual values here
    if (_level < 10) {
        _linesToNextLevel = (_level + 1) * 10;
    }
    else if (_level < 16) {
        _linesToNextLevel = 100;
    }
    else {
        _linesToNextLevel = (_level - 5) * 10;
    }
}

void DataManager::RuntimeData::addDataChangedCallback(std::function<void()> callback) {
    _dataChangedEvent->addListener(callback);
}

void DataManager::RuntimeData::removeDataChangedCallback(std::function<void()> callback) {
    _dataChangedEvent->removeListener(callback);
}