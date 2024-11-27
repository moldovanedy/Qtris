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

    private:
        PlayField();
        static PlayField *_instance;

        /**
         * Represents the play field, each bit is a square. Goes from top-left to bottom-right (e.g. row 1, column 3 = 14 (index 2, bit 6))
         */
        unsigned char _playField[25]{};
    };
}

#endif