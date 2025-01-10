#ifndef SELECT_LEVEL_H
#define SELECT_LEVEL_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QGridLayout>

#include "../main_window.h"

namespace UI::Menus {
    class SelectLevel : public QWidget {
        Q_OBJECT;

    public:
        explicit SelectLevel(QWidget *parent = nullptr);
        ~SelectLevel();

    protected:
        void keyPressEvent(QKeyEvent *e) override;

    protected slots:
        void selectLevelSlot(uint32_t selected);

    private:
        uint32_t _selectedLevel = 0;
        uint32_t _lastSelectedLevel = 0;
        QPushButton *_buttons[20];

        QBoxLayout *getMainContent();
        void visuallyActivateLevel();
        void selectLevel();
    };
}
#endif