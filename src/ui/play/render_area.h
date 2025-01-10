#ifndef RENDER_AREA_H
#define RENDER_AREA_H

#include <QWidget>
#include <QPainter>
#include <functional>

#include "../../game_manager/play_field.h"
#include "../../game_manager/main_loop.h"
#include "../../data_manager/runtime_data.h"
#include "../../game_manager/current_piece.h"
#include "piece_manager.h"

namespace UI::Play
{
    class RenderArea : public QWidget
    {
        Q_OBJECT

    public:
        explicit RenderArea(QWidget *parent = nullptr);
        ~RenderArea();

        void redraw();
        void checkForPieceColorChange();

    protected:
        void paintEvent(QPaintEvent *) override;

    private:
        QString _currentFile;
        unsigned char *_lastExpandedLayout;
        bool _isGameOver = false;

        unsigned int _gameOverCurtainFrame = -16;
        unsigned int _gameOverCurtainRow = 0;

        std::function<void()> _redrawCallback;

        void paintBlocks(QPainter *painter, QRect renderArea);
        void drawGameOverCurtain(QPainter *painter, QRect renderArea);

        void onGameOver();
    };
}

#endif // RENDER_AREA_H