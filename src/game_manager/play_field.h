#ifndef PIECE_LAYOUT_H
#define PIECE_LAYOUT_H

namespace GameManager {
    //20x10 = 200 bits, 200 / 8 = 25 bytes
    constexpr int PLAY_FIELD_ARRAY_SIZE = 25;
    constexpr int NUMBER_OF_ROWS = 20;
    constexpr int NUMBER_OF_COLUMNS = 10;

    class PlayField {
    public:
        PlayField(PlayField &other) = delete;
        ~PlayField() {}
        static PlayField *getInstance();

        bool isSquareFree(int row, int column);
        int getSquareType(int row, int column);
        void setSquareType(int row, int column, int squareType);

    private:
        PlayField();
        static PlayField *_instance;

        /**
         * Represents the play field, each 2 bits is a square. Goes from top-left to bottom-right
         * (e.g. row 1, column 3 = 26 and 27 (1 * 2 * 10 + 3 * 2)(index 3, bits 2 and 3))
         */
        unsigned char _playField[50]{};
    };
}

#endif