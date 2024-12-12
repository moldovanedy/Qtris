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

    private:
        void onPieceLocked();
        void onUpdate();
        int _framesUntilRepaint = 0;

        QString _currentFile;
        QLabel *_pieceView;
    };
}
#endif // NEXT_PIECE_VIEW_H
