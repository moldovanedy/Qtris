#ifndef HIGH_SCORES_H
#define HIGH_SCORES_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>

#include "../main_window.h"

namespace UI::Menus {
    class HighScores : public QWidget {
        Q_OBJECT;

    public:
        explicit HighScores(QWidget *parent = nullptr);
        ~HighScores();

    private:
        QVBoxLayout *getMainContent();
    };
}
#endif