#ifndef STATISTICS_VIEW_H
#define STATISTICS_VIEW_H

#include <QWidget>
#include <QBoxLayout>

#include "play_area.h"
#include "piece_manager.h"
#include "../../data_manager/piece_data.h"

namespace UI
{
    class StatisticsView : public QWidget
    {
        Q_OBJECT

    public:
        explicit StatisticsView(QWidget *parent = nullptr);
        ~StatisticsView();

        void incrementPieceNumber(DataManager::PieceData::PieceType pieceType);
        int getPieceNumber(DataManager::PieceData::PieceType pieceType);

    private:
        std::map<DataManager::PieceData::PieceType, QLabel *> *_labels =
            new std::map<DataManager::PieceData::PieceType, QLabel *>();
        QString _currentFile;
    };
}

#endif // STATISTICS_VIEW_H