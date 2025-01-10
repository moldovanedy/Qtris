#ifndef PLAY_AREA_H
#define PLAY_AREA_H

#include <QWidget>
#include <QKeyEvent>
#include <QBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QFontDatabase>
#include <QMediaPlayer>
#include <QAudioOutput>

#include <string>

#include "../main_window.h"
#include "render_area.h"
#include "next_piece_view.h"
#include "statistics_view.h"

#include "../../game_manager/current_piece.h"
#include "../../data_manager/runtime_data.h"

namespace UI::Play
{
    class PlayArea : public QWidget
    {
        Q_OBJECT

    public:
        explicit PlayArea(QWidget *parent = nullptr);
        ~PlayArea();

        void setScore(int score);
        void setLineCount(int lines);
        void setLevelNumber(int level);
        void redraw();

    protected:
        void keyPressEvent(QKeyEvent *e) override;
        void keyReleaseEvent(QKeyEvent *e) override;

    private:
        void onDataChanged();
        void onUpdate();
        std::function<void()> _updateCallback;
        std::function<void()> _gameOverCallback;
        void onGameOver();

        bool _isGameOver = false;
        unsigned int _gameOverSceneRemainingFrames = 16 + (4 * 20);

        void createLeftBar(QBoxLayout *columnBox);
        void createRightBar(QBoxLayout *columnBox);

        RenderArea *_renderArea;
        QLabel *_scoreLabel;
        QLabel *_linesLabel;
        QLabel *_levelLabel;

        QMediaPlayer *_mediaPlayer;
        QMediaPlayer *_gameOverSoundPlayer;

        QAudioOutput *_audioOutput;

        QString _currentFile;
    };
}

#endif // PLAY_AREA_H