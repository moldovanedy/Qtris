#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include <QWidget>
#include <QTimer>

#include "../utils/event.h"

namespace GameManager {
    class MainLoop : public QWidget {
        Q_OBJECT

    public:
        MainLoop(MainLoop &other) = delete;
        static MainLoop *getInstance();

        void addUpdateListener(Utils::eventListener *callback);
        bool removeUpdateListener(Utils::eventListener *callback);

    private:
        MainLoop(QWidget *parent = nullptr);
        static MainLoop *_instance;

        Utils::Event *_updateEvent;
        QTimer *_timer;
        int _fastFrames = 0;
        void invokeEvent();
    };
}
#endif