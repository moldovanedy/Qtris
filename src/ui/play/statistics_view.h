#ifndef STATISTICS_VIEW_H
#define STATISTICS_VIEW_H

#include <QWidget>
#include <QBoxLayout>
#include <QImage>
#include <thread>

#include "play_area.h"
#include "piece_manager.h"
#include "../../data_manager/piece_data.h"
#include "../../game_manager/current_piece.h"

namespace UI::Play
{
    class StatisticsView : public QWidget
    {
        Q_OBJECT

    public:
        explicit StatisticsView(QWidget *parent = nullptr);
        ~StatisticsView();

        int getPieceNumber(DataManager::PieceData::PieceType pieceType);
        void incrementPieceNumber(DataManager::PieceData::PieceType pieceType);
        void redrawPieces();

    private:
        void onPieceLocked();
        void onUpdate();
        std::function<void()> _updateCallback;
        std::function<void()> _redrawPiecesCallback;
        unsigned int _lastLevel = 0;
        bool _needsRepaint = false;

        std::map<DataManager::PieceData::PieceType, QLabel *> *_labels =
            new std::map<DataManager::PieceData::PieceType, QLabel *>();
        std::map<DataManager::PieceData::PieceType, QLabel *> *_images =
            new std::map<DataManager::PieceData::PieceType, QLabel *>();
        QString _currentFile;
    };
}

#endif // STATISTICS_VIEW_H