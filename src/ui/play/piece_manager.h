#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <QImage>
#include "../../data_manager/piece_data.h"

namespace UI::Resources
{
    /**
     * Returns the image for the specified type and the specified rotation (0 = 0deg, 1 = 90deg etc.).
     * If an image for the specified configuration wasn't created, it will be created and cached.
     * The returned pointer should NOT be deleted.
     *
     * @param pieceType: The piece type wanted.
     * @param rotation: The desired rotation (0 for 0 degrees, 1 for 90 degrees etc.) Set to a negative value to get
     * the default rotation (depends for each piece).
     * @param spawnPoint Receives a reference that will indicate the spawnPoint in image coordinates.
     */
    QImage *getPieceImage(DataManager::PieceData::PieceType pieceType, int rotation, QPoint &spawnPoint);

    unsigned char getColorOneFromPallette();
    unsigned char getColorTwoFromPallette();
    unsigned char getColorThreeFromPallette();
    unsigned char getColorFourFromPallette();

    void setColorOneFromPallette(unsigned char);
    void setColorTwoFromPallette(unsigned char);
    void setColorThreeFromPallette(unsigned char);
    void setColorFourFromPallette(unsigned char);

    /**
     * Do NOT use the given image inside UI because it will get deleted internally when the cache is invalidated.
     * Do NOT delete the image from other functions except inside this function.
     */
    QImage *getTypeOneBlock();
    /**
     * Do NOT use the given image inside UI because it will get deleted internally when the cache is invalidated.
     * Do NOT delete the image from other functions except inside this function.
     */
    QImage *getTypeTwoBlock();
    /**
     * Do NOT use the given image inside UI because it will get deleted internally when the cache is invalidated.
     * Do NOT delete the image from other functions except inside this function.
     */
    QImage *getTypeThreeBlock();

    /**
     * @param blockIndexType It is 1 for type one block, 2 for type two block and 3 for type three block.
     */
    static void paintImage(QImage *image, int blockType);
    static void recreateCachedImagesForPiece();
}

#endif // RESOURCE_MANAGER_H