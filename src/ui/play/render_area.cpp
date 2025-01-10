#include "render_area.h"

using namespace UI::Play;
using namespace GameManager;
using namespace DataManager;

RenderArea::RenderArea(QWidget *parent) : QWidget(parent) {
    //_lastExpandedLayout = new unsigned char[200] {0};
    this->setContentsMargins(0, 0, 0, 0);

    _redrawCallback = std::bind(&RenderArea::redraw, this);
    MainLoop::getInstance()->addUpdateEventListener(_redrawCallback);
    CurrentPiece::getInstance()->addGameOverEventListener(std::bind(&RenderArea::onGameOver, this));
    RuntimeData::addDataChangedCallback(std::bind(&RenderArea::checkForPieceColorChange, this));

    //change the colors of the pieces (because we might start from a level different than 0)
    this->checkForPieceColorChange();
}

RenderArea::~RenderArea() {
    MainLoop::getInstance()->removeUpdateEventListener(_redrawCallback);
}

void RenderArea::redraw() {
    this->update();
}

void RenderArea::checkForPieceColorChange() {
    //set the colors (will check if the level has changed or not)
    Resources::setColorsForLevel(RuntimeData::getLevel());
}

void RenderArea::paintEvent(QPaintEvent *e) {
    QRect renderArea = QRect(10, 20, this->width() - 20, this->height() - 40);
    QPainter painter(this);

    painter.setPen(Qt::PenStyle::SolidLine);
    //border
    painter.setBrush(QBrush(QColor(0x4f, 0xc3, 0xf7)));
    painter.drawRect(QRect(0, 0, this->width(), this->height()));

    //clear area
    painter.setBrush(QBrush(QColor(0x00, 0x00, 0x00)));
    painter.drawRect(renderArea);

    this->paintBlocks(&painter, renderArea);
    if (_isGameOver) {
        this->drawGameOverCurtain(&painter, renderArea);
    }
    else {
        ((QWidget *)this->parent())->setFocus();
    }
}

void RenderArea::paintBlocks(QPainter *painter, QRect renderArea) {
    float blockSize = renderArea.width() / 10.0;

    QImage typeOneImage, typeTwoImage, typeThreeImage;

    for (int row = 0; row < GameManager::NUMBER_OF_ROWS; row++) {
        for (int column = 0; column < GameManager::NUMBER_OF_COLUMNS; column++) {
            int blockType = PlayField::getInstance()->getSquareType(row, column);
            assert((blockType >= 0 && blockType <= 3) && "ERROR: block type is invalid");

            QImage imageToDraw;
            switch (blockType) {
            default:
            case 0:
                imageToDraw = QImage();
                break;
            case 1:
                if (typeOneImage.isNull()) {
                    QImage image = QImage(*(Resources::getTypeOneBlock()));
                    typeOneImage = image.scaled(QSize(blockSize, blockSize));
                }

                imageToDraw = typeOneImage;
                break;
            case 2:
                if (typeTwoImage.isNull()) {
                    QImage image = QImage(*(Resources::getTypeTwoBlock()));
                    typeTwoImage = image.scaled(QSize(blockSize, blockSize));
                }

                imageToDraw = typeTwoImage;
                break;
            case 3:
                if (typeThreeImage.isNull()) {
                    QImage image = QImage(*(Resources::getTypeThreeBlock()));
                    typeThreeImage = image.scaled(QSize(blockSize, blockSize));
                }

                imageToDraw = typeThreeImage;
                break;
            }

            QPoint drawPoint = QPoint(renderArea.x() + column * blockSize, renderArea.y() + row * blockSize);
            painter->drawImage(drawPoint, imageToDraw);
        }
    }
}

void RenderArea::drawGameOverCurtain(QPainter *painter, QRect renderArea) {
    float rowHeight = renderArea.height() / 20;

    if (_gameOverCurtainFrame < 4) {
        _gameOverCurtainFrame++;
    }
    else {
        _gameOverCurtainFrame = 0;

        if (_gameOverCurtainRow < 20) {
            _gameOverCurtainRow++;
        }
    }

    painter->setPen(Qt::PenStyle::SolidLine);
    for (int i = 0; i < _gameOverCurtainRow; i++) {
        painter->setBrush(QBrush(Resources::getColorFromNesPalette(Resources::getColorTwoFromPallette())));
        painter->drawRect(QRect(
            renderArea.x(),
            renderArea.y() + (i * rowHeight),
            renderArea.width(),
            rowHeight * 0.33));

        painter->setBrush(QBrush(Resources::getColorFromNesPalette(Resources::getColorThreeFromPallette())));
        painter->drawRect(QRect(
            renderArea.x(),
            renderArea.y() + (i * rowHeight) + (rowHeight * 0.33),
            renderArea.width(),
            rowHeight * 0.33));

        painter->setBrush(QBrush(Resources::getColorFromNesPalette(Resources::getColorFourFromPallette())));
        painter->drawRect(QRect(
            renderArea.x(),
            renderArea.y() + (i * rowHeight) + (rowHeight * 0.66),
            renderArea.width(),
            rowHeight * 0.33));
    }
}

void RenderArea::onGameOver() {
    _isGameOver = true;
}