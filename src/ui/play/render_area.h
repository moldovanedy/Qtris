#ifndef RENDER_AREA_H
#define RENDER_AREA_H

#include <QWidget>
#include <QPainter>
#include <functional>

#include "../../game_manager/play_field.h"
#include "../../game_manager/main_loop.h"
#include "piece_manager.h"

namespace UI
{
    class RenderArea : public QWidget
    {
        Q_OBJECT

    public:
        explicit RenderArea(QWidget *parent = nullptr);
        ~RenderArea();

        void redraw();

    protected:
        void paintEvent(QPaintEvent *) override;

    private:
        QString _currentFile;
        unsigned char *_lastExpandedLayout;

        void paintBlocks(QPainter *painter, QRect renderArea);
    };
}

#endif // RENDER_AREA_H