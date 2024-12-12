#include "next_piece_view.h"

UI::NextPieceView::NextPieceView(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    this->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    this->setContentsMargins(0, 0, 0, 0);

    this->setMaximumWidth(240);
    // 50 for "NEXT", 100 for piece drawing, 10 for padding (top and bottom)
    this->setMinimumHeight(50 + 10 + 100 + 10);

    QLabel *nextLabel = new QLabel(this);
    nextLabel->setStyleSheet("border: none");
    nextLabel->setText("NEXT");
    nextLabel->setStyleSheet("font-size: 26px;");
    nextLabel->setAlignment(Qt::AlignCenter);
    nextLabel->setFont(UI::PlayArea::getDataPixelFont());
    nextLabel->setFixedWidth(240);

    this->_pieceView = new QLabel(this);
    this->_pieceView->setGeometry(60, 80, 120, 60);
    this->_pieceView->setStyleSheet("border: none; margin: 0; padding: 0;");
    this->_pieceView->setMinimumWidth(64);
    this->_pieceView->setAlignment(Qt::AlignCenter);
    this->_pieceView->setContentsMargins(0, 0, 0, 0);


    GameManager::MainLoop::getInstance()->addUpdateEventListener(
        std::bind(&UI::NextPieceView::onUpdate, this));
    GameManager::CurrentPiece::getInstance()->addPieceLockedEventHandler(
        std::bind(&UI::NextPieceView::onPieceLocked, this));
}

UI::NextPieceView::~NextPieceView() {}

void UI::NextPieceView::onUpdate() {
    if (this->_framesUntilRepaint > 0) {
        this->_framesUntilRepaint--;
        return;
    }

    DataManager::PieceData::PieceType pieceType;
    int rotation;
    GameManager::CurrentPiece::getInstance()->getNextPieceData(pieceType, rotation);

    QPoint spawnPoint = QPoint();
    QImage *piece = UI::Resources::getPieceImage(pieceType, -1, spawnPoint);

    QImage scaledPiece;
    if (pieceType == DataManager::PieceData::PieceType::I) {
        scaledPiece = piece->scaled(QSize(30, 30), Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    }
    else {
        scaledPiece = piece->scaled(QSize(60, 60), Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    }

    this->_pieceView->setPixmap(QPixmap::fromImage(scaledPiece));
}

void UI::NextPieceView::onPieceLocked()
{
    this->_framesUntilRepaint = 2;
}