#ifndef RENDER_AREA_H
#define RENDER_AREA_H

#include <QWidget>
#include <QPainter>

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
    };
}

#endif // RENDER_AREA_H