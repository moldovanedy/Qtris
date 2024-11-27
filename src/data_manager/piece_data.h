#ifndef PIECE_DATA_H
#define PIECE_DATA_H

static int _defaultRotations[7]{ 2, 3, 0, 0, 0, 1, 1 };

namespace DataManager::PieceData {
    enum PieceType {
        T = 0,
        J = 1,
        Z = 2,
        O = 3,
        S = 4,
        L = 5,
        I = 6
    };

    /**
     * Provides the starting and ending x and y values from the layout (between 0 and 4 inclusive). Both the start and the end have a block
     * (e.g. startX = 2, endX = 2 means there are blocks only on the third column).
     */
    void getLayoutBounds(unsigned char *layout, int &startX, int &startY, int &endX, int &endY);

    /**
     * Returns the default rotation of the given piece type.
     */
    int getDefaultRotationForPiece(PieceType type);

    /**
     * Returns a byte array with 25 elements (5x5), where 0 means there is no piece and 1-3 means that type of piece.
     * The returned array should be deleted after use.
     *
     * @param pieceType: The piece type wanted.
     * @param rotation: The desired rotation (0 for 0 degrees, 1 for 90 degrees etc.). Set to a negative value to get
     * the default rotation (depends for each piece).
     */
    unsigned char *getPieceLayout(DataManager::PieceData::PieceType type, int rotation);
}
#endif