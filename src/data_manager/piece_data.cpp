#include "piece_data.h"

using namespace DataManager::PieceData;

unsigned char *DataManager::PieceData::getPieceLayout(DataManager::PieceData::PieceType type, int rotation) {
    if (rotation >= 0) {
        rotation = rotation % 4;
    }

    switch (type)
    {
    default:
    case PieceType::T:
    {
        switch (rotation)
        {
        case 0:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 1, 0, 0,
                    0, 1, 1, 1, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                };
        case 1:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 1, 0, 0,
                    0, 0, 1, 1, 0,
                    0, 0, 1, 0, 0,
                    0, 0, 0, 0, 0,
                };
        default:
        case 2:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 1, 1, 1, 0,
                    0, 0, 1, 0, 0,
                    0, 0, 0, 0, 0,
                };
        case 3:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 1, 0, 0,
                    0, 1, 1, 0, 0,
                    0, 0, 1, 0, 0,
                    0, 0, 0, 0, 0,
                };
        }

        break;
    }
    case PieceType::J:
    {
        switch (rotation)
        {
        case 0:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0,
                    0, 0, 3, 0, 0,
                    0, 3, 3, 0, 0,
                    0, 0, 0, 0, 0,
                };
        case 1:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 3, 0, 0, 0,
                    0, 3, 3, 3, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                };
        case 2:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 3, 3, 0,
                    0, 0, 3, 0, 0,
                    0, 0, 3, 0, 0,
                    0, 0, 0, 0, 0,
                };
        default:
        case 3:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 3, 3, 3, 0,
                    0, 0, 0, 3, 0,
                    0, 0, 0, 0, 0,
                };
        }

        break;
    }
    case PieceType::Z:
    {
        switch (rotation)
        {
        default:
        case 0:
        case 2:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 2, 2, 0, 0,
                    0, 0, 2, 2, 0,
                    0, 0, 0, 0, 0,
                };
        case 1:
        case 3:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 0, 2, 0,
                    0, 0, 2, 2, 0,
                    0, 0, 2, 0, 0,
                    0, 0, 0, 0, 0,
                };
        }

        break;
    }
    case PieceType::O:
    {
        return new unsigned char[25] {
            /**/0, 0, 0, 0, 0,
                0, 0, 0, 0, 0,
                0, 1, 1, 0, 0,
                0, 1, 1, 0, 0,
                0, 0, 0, 0, 0,
            };
    }
    case PieceType::S:
    {
        switch (rotation)
        {
        default:
        case 0:
        case 2:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 3, 3, 0,
                    0, 3, 3, 0, 0,
                    0, 0, 0, 0, 0,
                };
        case 1:
        case 3:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 3, 0, 0,
                    0, 0, 3, 3, 0,
                    0, 0, 0, 3, 0,
                    0, 0, 0, 0, 0,
                };
        }

        break;
    }
    case PieceType::L:
    {
        switch (rotation)
        {
        case 0:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 2, 0, 0,
                    0, 0, 2, 0, 0,
                    0, 0, 2, 2, 0,
                    0, 0, 0, 0, 0,
                };
        default:
        case 1:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    0, 2, 2, 2, 0,
                    0, 2, 0, 0, 0,
                    0, 0, 0, 0, 0,
                };
        case 2:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 2, 2, 0, 0,
                    0, 0, 2, 0, 0,
                    0, 0, 2, 0, 0,
                    0, 0, 0, 0, 0,
                };
        case 3:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 0, 2, 0,
                    0, 2, 2, 2, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                };
        }

        break;
    }
    case PieceType::I:
    {
        switch (rotation)
        {
        case 0:
        case 2:
            return new unsigned char[25] {
                /**/0, 0, 1, 0, 0,
                    0, 0, 1, 0, 0,
                    0, 0, 1, 0, 0,
                    0, 0, 1, 0, 0,
                    0, 0, 0, 0, 0,
                };
        default:
        case 1:
        case 3:
            return new unsigned char[25] {
                /**/0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                    1, 1, 1, 1, 0,
                    0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0,
                };
        }

        break;
    }
    }
}

void DataManager::PieceData::getLayoutBounds(unsigned char *layout, int &startX, int &startY, int &endX, int &endY) {
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column++) {
            if (layout[row * 5 + column] != 0) {
                if (column < startX) {
                    startX = column;
                }
                if (column > endX) {
                    endX = column;
                }

                if (row < startY) {
                    startY = row;
                }
                if (row > endY) {
                    endY = row;
                }
            }
        }
    }
}

int DataManager::PieceData::getDefaultRotationForPiece(DataManager::PieceData::PieceType type) {
    return _defaultRotations[(int)type];
    // switch (type) {
    // default:
    // case DataManager::PieceType::T:
    //     return _defaultRotations[0];
    // case DataManager::PieceType::J:
    //     return _defaultRotations[1];
    // case DataManager::PieceType::Z:
    //     return _defaultRotations[2];
    // case DataManager::PieceType::O:
    //     return _defaultRotations[3];
    // case DataManager::PieceType::S:
    //     return _defaultRotations[4];
    // case DataManager::PieceType::L:
    //     return _defaultRotations[5];
    // case DataManager::PieceType::I:
    //     return _defaultRotations[6];
    // }
}