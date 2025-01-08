#ifndef RUNTIME_DATA_H
#define RUNTIME_DATA_H

#include <functional>
#include <cmath>

#include "../utils/event.h"

namespace DataManager::RuntimeData {
    unsigned int getLevel();
    unsigned int getScore();
    unsigned int getClearedLines();
    unsigned int getGravitySpeed();

    void addClearedLines(unsigned int lines);
    void addSoftDropScore(unsigned int score);
    void setStartLevel(unsigned int level);

    void addDataChangedCallback(std::function<void()> callback);
    void removeDataChangedCallback(std::function<void()> callback);
    void terminate();
}
#endif