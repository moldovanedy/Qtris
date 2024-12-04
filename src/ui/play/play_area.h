#ifndef PLAY_AREA_H
#define PLAY_AREA_H

#include <QWidget>
#include <QKeyEvent>
#include <QBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QFontDatabase>

#include <string>

#include "main_view.h"
#include "render_area.h"
#include "next_piece_view.h"
#include "statistics_view.h"

#include "../../game_manager/current_piece.h"

namespace UI
{
    class PlayArea : public QWidget
    {
        Q_OBJECT

    public:
        explicit PlayArea(QWidget *parent = nullptr);
        ~PlayArea();

        static QFont getDataPixelFont();

        void setScore(int score);
        void setLineCount(int lines);
        void setLevelNumber(int level);
        void redraw();

    protected:
        void paintEvent(QPaintEvent *) override;
        void keyPressEvent(QKeyEvent *e) override;
        void keyReleaseEvent(QKeyEvent *e) override;

    private:
        static QFont _pixelFont;

        void createLeftBar(QBoxLayout *columnBox);
        void createRightBar(QBoxLayout *columnBox);

        RenderArea *_renderArea;
        QLabel *_scoreLabel;
        QLabel *_linesLabel;
        QLabel *_levelLabel;

        QString _currentFile;
    };
}

#endif // PLAY_AREA_H