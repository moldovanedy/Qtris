#ifndef RUNTIME_DATA_H
#define RUNTIME_DATA_H

namespace DataManager::RuntimeData {
    unsigned int getLevel();
    void setLevel(unsigned int level);

    unsigned int getScore();
    void setScore(unsigned int score);

    unsigned int getClearedLines();
    void setClearedLines(unsigned int lines);
}
#endif