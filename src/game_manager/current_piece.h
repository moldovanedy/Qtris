#ifndef CURRENT_PIECE_H
#define CURRENT_PIECE_H

#include <QMediaPlayer>
#include <QAudioOutput>

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
        ~CurrentPiece();

        void getCurrentPieceData(PieceType &pieceType, int &rotation);
        void getNextPieceData(PieceType &pieceType, int &rotation);

        inline void setSoftDropKeyState(bool isPressed) {
            //NES IMPLEMENTATION: moving a piece will not allow soft drop
            if (_isMoveLeftKeyDown || _isMoveRightKeyDown) {
                return;
            }

            _isSoftDropKeyDown = isPressed;
            //NES IMPLEMENTATION: 3 frames for drop to activate
            _softDropFrame = 3;

            if (!isPressed) {
                _fallFrame = 0;
            }
        }

        inline void setMoveLeftKeyState(bool isPressed) {
            //NES IMPLEMENTATION: soft dropping will not allow moving a piece
            if (_isSoftDropKeyDown) {
                return;
            }
            _isMoveLeftKeyDown = isPressed;

            if (isPressed) {
                _dasFrame = 16;
                this->moveToLeft();
            }
        }

        inline void setMoveRightKeyState(bool isPressed) {
            //NES IMPLEMENTATION: soft dropping will not allow moving a piece
            if (_isSoftDropKeyDown) {
                return;
            }
            _isMoveRightKeyDown = isPressed;

            if (isPressed) {
                _dasFrame = 16;
                this->moveToRight();
            }
        }

        inline void setRotateClockwiseKeyState(bool isPressed) {
            _isRotateClockwiseKeyDown = isPressed;

            if (isPressed) {
                this->rotateClockwise();
            }
        }

        inline void setRotateCounterClockwiseKeyState(bool isPressed) {
            _isRotateCounterClockwiseKeyDown = isPressed;

            if (isPressed) {
                this->rotateCounterClockwise();
            }
        }

        void setLineClearDelay(int frameDelay);

        /**
         * Will call the given eventListener (callback) when the piece is locked into the existing pieces or floor.
         */
        void addPieceLockedEventListener(std::function<void()> callback);
        bool removePieceLockedEventListener(std::function<void()> callback);

        /**
         * Will call the given eventListener (callback) when the it's game over.
         */
        void addGameOverEventListener(std::function<void()> callback);
        bool removeGameOverEventListener(std::function<void()> callback);

        void stop();
        void restart();

    private:
        CurrentPiece();
        static CurrentPiece *_instance;

        /**
         * Will get the next piece and will try to spawn it. If it fails, it means it's game over.
         */
        bool reset();
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
        Utils::Event *_gameOverEvent;

        int _rowPosition = 0;
        int _columnPosition = 5;

        int _startX = 0, _startY = 0, _endX = 0, _endY = 0;
        int _xPos = 5, _yPos = 0;
        PieceType _pieceType = PieceType::T;
        int _rotation = 0;

        PieceType _nextPieceType = PieceType::T;
        int _nextPieceRotation = 0;
        bool _isFirstPiece = true;

        int _lineClearRemainingFrames = 0;
        int _fallFrame = 0;
        //If -1, it means it was invalidated by the game and the only way to restart it is to perform a soft drop 
        //(intended behavior because holding for soft drop after the piece has been locked and ARE is done should NOT use soft drop)
        int _softDropFrame = -1;
        bool _isSoftDropKeyDown = false;
        //the number of frames the piece is soft-dropped (resets when the key is released)
        unsigned int _softDropCount = 0;

        int _dasFrame = 0;
        bool _isMoveLeftKeyDown = false;
        bool _isMoveRightKeyDown = false;

        bool _isRotateClockwiseKeyDown = false;
        bool _isRotateCounterClockwiseKeyDown = false;


        QMediaPlayer *_pieceLockedSoundPlayer;
        QMediaPlayer *_rotatePieceSoundPlayer;

        QAudioOutput *_sfxAudioOutput;
    };
}
#endif