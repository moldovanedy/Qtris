#ifndef RUNTIME_DATA_H
#define RUNTIME_DATA_H

#include <functional>

#include "../utils/event.h"

namespace DataManager::RuntimeData {
    unsigned int getLevel();
    unsigned int getScore();
    unsigned int getClearedLines();
    unsigned int getGravitySpeed();

    void addClearedLines(unsigned int lines);
    void addSoftDropScore(unsigned int score);

    void addDataChangedCallback(std::function<void()> callback);
    void removeDataChangedCallback(std::function<void()> callback);
}
#endif