#ifndef CURRENT_PIECE_H
#define CURRENT_PIECE_H

#include "../utils/event.h"
#include "../data_manager/piece_data.h"
#include "main_loop.h"
#include "play_field.h"
#include "../data_manager/runtime_data.h"

using namespace DataManager::PieceData;

namespace GameManager {
    class CurrentPiece {
    public:
        CurrentPiece(CurrentPiece &other) = delete;
        static CurrentPiece *getInstance();

        void getCurrentPieceData(PieceType &pieceType, int &rotation);
        void getNextPieceData(PieceType &pieceType, int &rotation);

        inline void setSoftDropKeyState(bool isPressed) {
            //NES IMPLEMENTATION: moving a piece will not allow soft drop
            if (this->_isMoveLeftKeyDown || this->_isMoveRightKeyDown) {
                return;
            }

            this->_isSoftDropKeyDown = isPressed;
            //NES IMPLEMENTATION: 3 frames for drop to activate
            this->_softDropFrame = 3;

            if (!isPressed) {
                this->_fallFrame = 0;
            }
        }

        inline void setMoveLeftKeyState(bool isPressed) {
            //NES IMPLEMENTATION: soft dropping will not allow moving a piece
            if (this->_isSoftDropKeyDown) {
                return;
            }
            this->_isMoveLeftKeyDown = isPressed;

            if (isPressed) {
                this->_dasFrame = 16;
                this->moveToLeft();
            }
        }

        inline void setMoveRightKeyState(bool isPressed) {
            //NES IMPLEMENTATION: soft dropping will not allow moving a piece
            if (this->_isSoftDropKeyDown) {
                return;
            }
            this->_isMoveRightKeyDown = isPressed;

            if (isPressed) {
                this->_dasFrame = 16;
                this->moveToRight();
            }
        }

        inline void setRotateClockwiseKeyState(bool isPressed) {
            this->_isRotateClockwiseKeyDown = isPressed;

            if (isPressed) {
                this->rotateClockwise();
            }
        }

        inline void setRotateCounterClockwiseKeyState(bool isPressed) {
            this->_isRotateCounterClockwiseKeyDown = isPressed;

            if (isPressed) {
                this->rotateCounterClockwise();
            }
        }

        /**
         * Will get the next piece and will try to spawn it. If it fails, it means it's game over.
         */
        bool reset();
        /**
         * Will call the given eventListener (callback) when the piece is locked into the existing pieces or floor.
         */
        void addPieceLockedEventHandler(Utils::eventListener *callback);
        bool removePieceLockedEventHandler(Utils::eventListener *callback);

    private:
        CurrentPiece();
        ~CurrentPiece() {};
        static CurrentPiece *_instance;

        /**
         * Will start the ARE time (the spawn delay) and will automatically call reset when needed.
         */
        void passControlToAre();
        /**
         * The current number of frames it needs to wait for ARE (it is decremented every frame). 0 means ARE is inactive.
         */
        int _areRemainingFrames = 96;
        bool _isAreActive = true;

        bool rotateCounterClockwise();
        bool rotateClockwise();
        void performSoftDrop();
        void moveToLeft();
        void moveToRight();

        void performFall();
        bool canMoveDown();
        bool canMoveLeft();
        bool canMoveRight();
        bool canRotateClockwise();
        bool canRotateCounterClockwise();

        PieceType generateNextPiece();
        void redoLayout();
        //an array of 4 elements that contains the indices of the piece from the last update
        int _lastUsedIndices[4];

        unsigned char *_cachedLayout = new unsigned char[25];

        Utils::Event *_pieceLockedEvent;
        std::function<void()> _updateCallback;
        void onUpdate();

        int _rowPosition = 0;
        int _columnPosition = 5;

        int _startX = 0, _startY = 0, _endX = 0, _endY = 0;
        int xPos = 5, yPos = 0;
        PieceType _pieceType = PieceType::T;
        int _rotation = 0;

        PieceType _nextPieceType = PieceType::T;
        int _nextPieceRotation = 0;
        bool _isFirstPiece = true;

        int _fallFrame = 0;
        //If -1, it means it was invalidated by the game and the only way to restart it is to perform a soft drop 
        //(intended behavior because holding for soft drop after the piece has been locked and ARE is done should NOT use soft drop)
        int _softDropFrame = -1;
        bool _isSoftDropKeyDown = false;

        int _dasFrame = 0;
        bool _isMoveLeftKeyDown = false;
        bool _isMoveRightKeyDown = false;

        bool _isRotateClockwiseKeyDown = false;
        bool _isRotateCounterClockwiseKeyDown = false;
    };
}
#endif