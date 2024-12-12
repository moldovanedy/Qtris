#include "piece_manager.h"

//initialized to the colors of level 1
static unsigned int _currentPalette = (0x0f << 24) | (0x30 << 16) | (0x21 << 8) | 0x12;

static int _nesPallette[64]{
    /**/0x7c7c7c, 0x0000fc, 0x0000bc, 0x4428bc, 0x940084, 0xa80020, 0xa81000, 0x881400,
        0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000,

        0xbcbcbc, 0x0078f8, 0x0058f8, 0x6844fc, 0xd800cc, 0xe40058, 0xf83800, 0xe45c10,
        0xac7c00, 0x00b800, 0x00a800, 0x00a844, 0x008888, 0x000000, 0x000000, 0x000000,

        0xf8f8f8, 0x3cbcfc, 0x6888fc, 0x9878f8, 0xf878f8, 0xf85898, 0xf87858, 0xfca044,
        0xf8b800, 0xb8f818, 0x58d854, 0x58f898, 0x00e8d8, 0x787878, 0x000000, 0x000000,

        0xfcfcfc, 0xa4e4fc, 0xb8b8f8, 0xd8b8f8, 0xf8b8f8, 0xf8a4c0, 0xf0d0b0, 0xfce0a8,
        0xf8d878, 0xd8f878, 0xb8f8b8, 0xb8f8d8, 0x00fcfc, 0xf8d8f8, 0x000000, 0x000000 };

static unsigned char _knownColors[30]{
    /**/0x30, 0x21, 0x12,
    0x30, 0x29, 0x1A,
    0x30, 0x24, 0x14,
    0x30, 0x2A, 0x12,
    0x30, 0x2B, 0x15,
    0x30, 0x22, 0x2B,
    0x30, 0x00, 0x16,
    0x30, 0x05, 0x13,
    0x30, 0x16, 0x12,
    0x30, 0x27, 0x16
};

static unsigned char _blockOneData[64]{
    /**/1, 3, 3, 3, 3, 3, 3, 0,
        3, 1, 1, 1, 1, 1, 3, 0,
        3, 1, 1, 1, 1, 1, 3, 0,
        3, 1, 1, 1, 1, 1, 3, 0,
        3, 1, 1, 1, 1, 1, 3, 0,
        3, 1, 1, 1, 1, 1, 3, 0,
        3, 3, 3, 3, 3, 3, 3, 0,
        0, 0, 0, 0, 0, 0, 0, 0
};

static unsigned char _blockTwoData[64]{
    /**/1, 2, 2, 2, 2, 2, 2, 0,
        2, 1, 1, 2, 2, 2, 2, 0,
        2, 1, 2, 2, 2, 2, 2, 0,
        2, 2, 2, 2, 2, 2, 2, 0,
        2, 2, 2, 2, 2, 2, 2, 0,
        2, 2, 2, 2, 2, 2, 2, 0,
        2, 2, 2, 2, 2, 2, 2, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
};

static unsigned char _blockThreeData[64]{
    /**/1, 3, 3, 3, 3, 3, 3, 0,
        3, 1, 1, 3, 3, 3, 3, 0,
        3, 1, 3, 3, 3, 3, 3, 0,
        3, 3, 3, 3, 3, 3, 3, 0,
        3, 3, 3, 3, 3, 3, 3, 0,
        3, 3, 3, 3, 3, 3, 3, 0,
        3, 3, 3, 3, 3, 3, 3, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
};

static QImage *_cachedBlockTypeOne;
static QImage *_cachedBlockTypeTwo;
static QImage *_cachedBlockTypeThree;

static QImage **_cachedTArray = new QImage * [4];
static QImage **_cachedJArray = new QImage * [4];
static QImage **_cachedZArray = new QImage * [4];
static QImage **_cachedOArray = new QImage * [4];
static QImage **_cachedSArray = new QImage * [4];
static QImage **_cachedLArray = new QImage * [4];
static QImage **_cachedIArray = new QImage * [4];

static bool _isCacheValid = false;

QImage *UI::Resources::getPieceImage(DataManager::PieceData::PieceType pieceType, int rotation, QPoint &spawnPoint) {
    if (!_isCacheValid) {
        recreateCachedImagesForPiece();
    }

    if (rotation < 0) {
        rotation = DataManager::PieceData::getDefaultRotationForPiece(pieceType);
    }

    switch (pieceType) {
    case DataManager::PieceData::PieceType::T:
        return _cachedTArray[rotation];
    case DataManager::PieceData::PieceType::J:
        return _cachedJArray[rotation];
    case DataManager::PieceData::PieceType::Z:
        return _cachedZArray[rotation];
    case DataManager::PieceData::PieceType::O:
        return _cachedOArray[rotation];
    case DataManager::PieceData::PieceType::S:
        return _cachedSArray[rotation];
    case DataManager::PieceData::PieceType::L:
        return _cachedLArray[rotation];
    case DataManager::PieceData::PieceType::I:
        return _cachedIArray[rotation];
    }
    return new QImage();
}

unsigned char UI::Resources::getColorOneFromPallette() {
    return _currentPalette >> 24;
}

unsigned char UI::Resources::getColorTwoFromPallette() {
    return _currentPalette >> 16;
}

unsigned char UI::Resources::getColorThreeFromPallette() {
    return _currentPalette >> 8;
}

unsigned char UI::Resources::getColorFourFromPallette() {
    return _currentPalette;
}

static void setColorOneFromPallette(unsigned char palletteValue) {
    if (palletteValue >= 0x40) {
        throw std::invalid_argument("Invalid pallette index at UI::Resources::setColorOneFromPallette");
    }

    _currentPalette = (_currentPalette & 0x00ffffff) | ((int)palletteValue << 24);
    _isCacheValid = false;
}

static void setColorTwoFromPallette(unsigned char palletteValue) {
    if (palletteValue >= 0x40) {
        throw std::invalid_argument("Invalid pallette index at UI::Resources::setColorTwoFromPallette");
    }

    _currentPalette = (_currentPalette & 0xff00ffff) | ((int)palletteValue << 16);
    _isCacheValid = false;
}

static void setColorThreeFromPallette(unsigned char palletteValue) {
    if (palletteValue >= 0x40) {
        throw std::invalid_argument("Invalid pallette index at UI::Resources::setColorThreeFromPallette");
    }

    _currentPalette = (_currentPalette & 0xffff00ff) | ((int)palletteValue << 8);
    _isCacheValid = false;
}

static void setColorFourFromPallette(unsigned char palletteValue) {
    if (palletteValue >= 0x40) {
        throw std::invalid_argument("Invalid pallette index at UI::Resources::setColorFourFromPallette");
    }

    _currentPalette = (_currentPalette & 0xffffff00) | (int)palletteValue;
    _isCacheValid = false;
}

unsigned int _lastLevel = 0;

void UI::Resources::setColorsForLevel(unsigned int level) {
    //TODO: also implement glitched colors
    if (_lastLevel == level) {
        return;
    }
    level = level % 10;
    _lastLevel = level;
    //qDebug() << level;

    setColorTwoFromPallette(_knownColors[level * 3]);
    setColorThreeFromPallette(_knownColors[level * 3 + 1]);
    setColorFourFromPallette(_knownColors[level * 3 + 2]);
}


QImage *UI::Resources::getTypeOneBlock() {
    if (!_isCacheValid)
    {
        _cachedBlockTypeOne = new QImage(QSize(8, 8), QImage::Format_RGB888);
        paintImage(_cachedBlockTypeOne, 1);
    }

    return _cachedBlockTypeOne;
}

QImage *UI::Resources::getTypeTwoBlock() {
    if (!_isCacheValid)
    {
        _cachedBlockTypeTwo = new QImage(QSize(8, 8), QImage::Format_RGB888);
        paintImage(_cachedBlockTypeTwo, 2);
    }

    return _cachedBlockTypeTwo;
}

QImage *UI::Resources::getTypeThreeBlock() {
    if (!_isCacheValid)
    {
        _cachedBlockTypeThree = new QImage(QSize(8, 8), QImage::Format_RGB888);
        paintImage(_cachedBlockTypeThree, 3);
    }

    return _cachedBlockTypeThree;
}

void UI::Resources::paintImage(QImage *image, int blockType) {
    unsigned char *data;
    switch (blockType)
    {
    default:
    case 1:
        data = _blockOneData;
        break;
    case 2:
        data = _blockTwoData;
        break;
    case 3:
        data = _blockThreeData;
        break;
    }

    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
            QColor color;
            switch (data[row * 8 + column]) {
            default:
            case 0:
                color = QColor(_nesPallette[getColorOneFromPallette()]);
                break;
            case 1:
                color = QColor(_nesPallette[getColorTwoFromPallette()]);
                break;
            case 2:
                color = QColor(_nesPallette[getColorThreeFromPallette()]);
                break;
            case 3:
                color = QColor(_nesPallette[getColorFourFromPallette()]);
                break;
            }

            image->setPixelColor(row, column, color);
        }
    }
}

static void UI::Resources::recreateCachedImagesForPiece() {
    for (int piece = 0; piece < 7; piece++) {
        DataManager::PieceData::PieceType pieceType = (DataManager::PieceData::PieceType)piece;

        for (int rot = 0; rot < 4; rot++) {
            unsigned char *layout = DataManager::PieceData::getPieceLayout(pieceType, rot);
            int startX = 4, startY = 4, endX = 0, endY = 0;
            DataManager::PieceData::getLayoutBounds(layout, startX, startY, endX, endY);

            assert((endX >= startX) && "endX smaller than startX");
            assert((endY >= startY) && "endY smaller than startY");

            QImage *finalImage = new QImage(QSize((endX - startX + 1) * 8, (endY - startY + 1) * 8), QImage::Format_RGB888);
            finalImage->fill(Qt::GlobalColor::transparent);

            for (int row = 0; row < 5; row++) {
                for (int column = 0; column < 5; column++) {
                    QImage *block;
                    //if no block, continue
                    if (layout[row * 5 + column] < 1 || layout[row * 5 + column] > 3) {
                        continue;
                    }

                    switch (layout[row * 5 + column])
                    {
                    case 1:
                        block = UI::Resources::getTypeOneBlock();
                        break;
                    case 2:
                        block = UI::Resources::getTypeTwoBlock();
                        break;
                    case 3:
                        block = UI::Resources::getTypeThreeBlock();
                        break;
                    }

                    //BitBlt
                    int xPositionOnImage = (column - startX) * 8,
                        yPositionOnImage = (row - startY) * 8;
                    for (int pxY = yPositionOnImage; pxY < 8 + yPositionOnImage; pxY++) {
                        for (int pxX = xPositionOnImage; pxX < 8 + xPositionOnImage; pxX++) {
                            finalImage->setPixel(pxX, pxY, block->pixel(pxX - xPositionOnImage, pxY - yPositionOnImage));
                        }
                    }
                }
            }

            switch (pieceType) {
            case DataManager::PieceData::PieceType::T:
                delete _cachedTArray[rot];
                _cachedTArray[rot] = finalImage;
                break;
            case DataManager::PieceData::PieceType::J:
                delete _cachedJArray[rot];
                _cachedJArray[rot] = finalImage;
                break;
            case DataManager::PieceData::PieceType::Z:
                delete _cachedZArray[rot];
                _cachedZArray[rot] = finalImage;
                break;
            case DataManager::PieceData::PieceType::O:
                delete _cachedOArray[rot];
                _cachedOArray[rot] = finalImage;
                break;
            case DataManager::PieceData::PieceType::S:
                delete _cachedSArray[rot];
                _cachedSArray[rot] = finalImage;
                break;
            case DataManager::PieceData::PieceType::L:
                delete _cachedLArray[rot];
                _cachedLArray[rot] = finalImage;
                break;
            case DataManager::PieceData::PieceType::I:
                delete _cachedIArray[rot];
                _cachedIArray[rot] = finalImage;
                break;
            }

            delete[] layout;
        }
    }

    _isCacheValid = true;
}