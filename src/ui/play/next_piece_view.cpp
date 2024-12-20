#include "next_piece_view.h"

using namespace UI::Play;
using namespace GameManager;
using namespace DataManager;

NextPieceView::NextPieceView(QWidget *parent) : QWidget(parent)
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
    nextLabel->setFont(MainWindow::getInstance()->getAppFont());
    nextLabel->setFixedWidth(240);

    _pieceView = new QLabel(this);
    _pieceView->setGeometry(60, 80, 120, 60);
    _pieceView->setStyleSheet("border: none; margin: 0; padding: 0;");
    _pieceView->setMinimumWidth(64);
    _pieceView->setAlignment(Qt::AlignCenter);
    _pieceView->setContentsMargins(0, 0, 0, 0);


    MainLoop::getInstance()->addUpdateEventListener(
        std::bind(&NextPieceView::onUpdate, this));
    CurrentPiece::getInstance()->addPieceLockedEventHandler(
        std::bind(&NextPieceView::onPieceLocked, this));
}

NextPieceView::~NextPieceView() {}

void NextPieceView::onUpdate() {
    if (_framesUntilRepaint > 0) {
        _framesUntilRepaint--;
        return;
    }

    PieceData::PieceType pieceType;
    int rotation;
    CurrentPiece::getInstance()->getNextPieceData(pieceType, rotation);

    QPoint spawnPoint = QPoint();
    QImage *piece = Resources::getPieceImage(pieceType, -1, spawnPoint);

    QImage scaledPiece;
    if (pieceType == PieceData::PieceType::I) {
        scaledPiece = piece->scaled(QSize(30, 30), Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    }
    else {
        scaledPiece = piece->scaled(QSize(60, 60), Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    }

    _pieceView->setPixmap(QPixmap::fromImage(scaledPiece));
}

void NextPieceView::onPieceLocked()
{
    _framesUntilRepaint = 2;
}