#ifndef CURRENT_PIECE_H
#define CURRENT_PIECE_H

#include "utils/event.h"
#include "../data_manager/piece_data.h"

namespace GameManager {
    class CurrentPiece {
    public:
        CurrentPiece(CurrentPiece &other) = delete;
        ~CurrentPiece() {};
        static CurrentPiece *getInstance();

        bool reset();
        bool rotateCounterClockwise();
        bool rotateClockwise();
        void performSoftDrop();
        void performFall();
        /**
         * Will call the given eventListener (callback) when the piece is locked into the existing pieces or floor.
         */
        void addPieceLockedEventHandler(Utils::eventListener *callback);
        bool removePieceLockedEventHandler(Utils::eventListener *callback);

    private:
        CurrentPiece();
        static CurrentPiece *_instance;

        Utils::Event *_pieceLockedEvent;
        int _rowPosition = 0;
        int _columnPosition = 5;

        int _startX = 0, _startY = 0, _endX = 0, _endY = 0;
        DataManager::PieceType _pieceType = DataManager::PieceType::T;
        int _rotation = 0;
    };
}
#endif //CURRENT_PIECE_H