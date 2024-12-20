#include "render_area.h"

using namespace UI::Play;
using namespace GameManager;
using namespace DataManager;

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    //_lastExpandedLayout = new unsigned char[200] {0};
    this->setContentsMargins(0, 0, 0, 0);

    MainLoop::getInstance()->addUpdateEventListener(std::bind(&RenderArea::redraw, this));
    RuntimeData::addDataChangedCallback(std::bind(&RenderArea::checkForPieceColorChange, this));
}

RenderArea::~RenderArea() {
    //delete[] _lastExpandedLayout;
}

void RenderArea::redraw()
{
    this->update();
}

void RenderArea::checkForPieceColorChange() {
    //set the colors (will check if the level has changed or not)
    Resources::setColorsForLevel(RuntimeData::getLevel());
}

void RenderArea::paintEvent(QPaintEvent *e)
{
    QRect renderArea = QRect(10, 20, this->width() - 20, this->height() - 40);
    QPainter painter(this);

    painter.setPen(Qt::PenStyle::SolidLine);
    //border
    painter.setBrush(QBrush(QColor(0x00, 0xff, 0xff)));
    painter.drawRect(QRect(0, 0, this->width(), this->height()));

    //clear area
    painter.setBrush(QBrush(QColor(0x00, 0x00, 0x00)));
    painter.drawRect(renderArea);

    this->paintBlocks(&painter, renderArea);
    ((QWidget *)this->parent())->setFocus();
}

void RenderArea::paintBlocks(QPainter *painter, QRect renderArea) {
    float blockSize = renderArea.width() / 10.0;

    QImage typeOneImage, typeTwoImage, typeThreeImage;

    for (int row = 0; row < GameManager::NUMBER_OF_ROWS; row++) {
        for (int column = 0; column < GameManager::NUMBER_OF_COLUMNS; column++) {
            int blockType = PlayField::getInstance()->getSquareType(row, column);
            assert((blockType >= 0 && blockType <= 3) && "ERROR: block type is invalid");

            // if (blockType == _lastExpandedLayout[row * GameManager::NUMBER_OF_COLUMNS + column]) {
            //     continue;
            // }
            // _lastExpandedLayout[row * GameManager::NUMBER_OF_COLUMNS + column] = blockType;

            QImage imageToDraw;
            switch (blockType) {
            default:
            case 0:
                imageToDraw = QImage();
                break;
            case 1:
                //TODO: we need to check if we need to regenerate the images somehow here
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