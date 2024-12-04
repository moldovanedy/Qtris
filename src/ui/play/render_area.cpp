#include "render_area.h"

UI::RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    //this->_lastExpandedLayout = new unsigned char[200] {0};
    this->setContentsMargins(0, 0, 0, 0);

    GameManager::MainLoop::getInstance()->addUpdateEventListener(std::bind(&UI::RenderArea::redraw, this));
}

UI::RenderArea::~RenderArea() {
    //delete[] this->_lastExpandedLayout;
}

void UI::RenderArea::redraw()
{
    this->update();
}

void UI::RenderArea::paintEvent(QPaintEvent *e)
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
}

void UI::RenderArea::paintBlocks(QPainter *painter, QRect renderArea) {
    float blockSize = renderArea.width() / 10.0;

    QImage typeOneImage, typeTwoImage, typeThreeImage;

    for (int row = 0; row < GameManager::NUMBER_OF_ROWS; row++) {
        for (int column = 0; column < GameManager::NUMBER_OF_COLUMNS; column++) {
            int blockType = GameManager::PlayField::getInstance()->getSquareType(row, column);
            assert((blockType >= 0 && blockType <= 3) && "ERROR: block type is invalid");

            // if (blockType == this->_lastExpandedLayout[row * GameManager::NUMBER_OF_COLUMNS + column]) {
            //     continue;
            // }
            // this->_lastExpandedLayout[row * GameManager::NUMBER_OF_COLUMNS + column] = blockType;

            QImage imageToDraw;
            switch (blockType) {
            default:
            case 0:
                imageToDraw = QImage();
                break;
            case 1:
                if (typeOneImage.isNull()) {
                    QImage image = QImage(*(UI::Resources::getTypeOneBlock()));
                    typeOneImage = image.scaled(QSize(blockSize, blockSize));
                }

                imageToDraw = typeOneImage;
                break;
            case 2:
                if (typeTwoImage.isNull()) {
                    QImage image = QImage(*(UI::Resources::getTypeTwoBlock()));
                    typeTwoImage = image.scaled(QSize(blockSize, blockSize));
                }

                imageToDraw = typeTwoImage;
                break;
            case 3:
                if (typeThreeImage.isNull()) {
                    QImage image = QImage(*(UI::Resources::getTypeThreeBlock()));
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