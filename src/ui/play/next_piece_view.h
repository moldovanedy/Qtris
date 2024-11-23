#ifndef NEXT_PIECE_VIEW_H
#define NEXT_PIECE_VIEW_H

#include <QWidget>

#include "main_view.h"
#include "play_area.h"

namespace UI
{
    class NextPieceView : public QWidget
    {
        Q_OBJECT

    public:
        explicit NextPieceView(QWidget *parent = nullptr);
        ~NextPieceView();

    protected:
        void paintEvent(QPaintEvent *) override;

    private:
        QString _currentFile;
    };
}
#endif // NEXT_PIECE_VIEW_H
