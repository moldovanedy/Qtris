#include "statistics_view.h"

using namespace UI::Play;
using namespace GameManager;
using namespace DataManager;

StatisticsView::StatisticsView(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    this->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    this->setContentsMargins(0, 0, 0, 0);
    this->setMaximumWidth(300);

    QBoxLayout *mainColumn = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);

    QLabel *title = new QLabel(this);
    title->setStyleSheet("border: none");
    title->setFont(MainWindow::getInstance()->getAppFont());
    title->setText("STATISTICS");
    title->setContentsMargins(0, 0, 0, 0);
    title->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    title->setMaximumWidth(300);

    mainColumn->addWidget(title);

    for (int i = 0; i < 7; i++) {
        PieceData::PieceType pieceType = (PieceData::PieceType)i;

        QBoxLayout *row = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
        row->setAlignment(Qt::AlignCenter);

        QPoint spawnPoint = QPoint();
        QImage *piece = Resources::getPieceImage(pieceType, -1, spawnPoint);
        QImage scaledPiece;
        if (pieceType == PieceData::PieceType::I) {
            scaledPiece = piece->scaled(QSize(16, 16), Qt::AspectRatioMode::KeepAspectRatioByExpanding);
        }
        else {
            scaledPiece = piece->scaled(QSize(32, 32), Qt::AspectRatioMode::KeepAspectRatioByExpanding);
        }

        QLabel *imageView = new QLabel();
        imageView->setStyleSheet("border: none; margin: 0; padding: 0;");
        imageView->setMinimumWidth(64);
        imageView->setAlignment(Qt::AlignCenter);
        imageView->setContentsMargins(0, 0, 0, 0);
        imageView->setPixmap(QPixmap::fromImage(scaledPiece));
        _images->insert(std::make_pair(pieceType, imageView));

        row->addWidget(imageView);

        QLabel *statLabel = new QLabel(this);
        statLabel->setText("000");
        statLabel->setContentsMargins(0, 0, 0, 0);
        statLabel->setFont(MainWindow::getInstance()->getAppFont());
        statLabel->setStyleSheet("border: none");

        row->addWidget(statLabel);
        _labels->insert(std::make_pair(pieceType, statLabel));

        mainColumn->addLayout(row);
    }


    _updateCallback = std::bind(&StatisticsView::onUpdate, this);

    MainLoop::getInstance()->addUpdateEventListener(_updateCallback);
    CurrentPiece::getInstance()->addPieceLockedEventListener(std::bind(&StatisticsView::onPieceLocked, this));
    RuntimeData::addDataChangedCallback(std::bind(&StatisticsView::redrawPieces, this));

    //change the colors of the pieces (because we might start from a level different than 0)
    this->redrawPieces();
}

StatisticsView::~StatisticsView() {
    MainLoop::getInstance()->removeUpdateEventListener(_updateCallback);
}

void StatisticsView::redrawPieces() {
    if (RuntimeData::getLevel() == _lastLevel) {
        return;
    }
    _lastLevel = RuntimeData::getLevel();
    _needsRepaint = true;
}

void StatisticsView::onUpdate() {
    if (!_needsRepaint) {
        return;
    }

    for (int i = 0; i < 7; i++) {
        PieceData::PieceType pieceType = (PieceData::PieceType)i;

        QPoint spawnPoint = QPoint();
        QImage *piece = Resources::getPieceImage(pieceType, -1, spawnPoint);
        QImage scaledPiece;
        if (pieceType == PieceData::PieceType::I) {
            scaledPiece = piece->scaled(QSize(16, 16), Qt::AspectRatioMode::KeepAspectRatioByExpanding);
        }
        else {
            scaledPiece = piece->scaled(QSize(32, 32), Qt::AspectRatioMode::KeepAspectRatioByExpanding);
        }
        //delete piece;

        _images->at(pieceType)->setPixmap(QPixmap::fromImage(scaledPiece));
    }
    _needsRepaint = false;
}

void StatisticsView::incrementPieceNumber(PieceData::PieceType pieceType) {
    int pieceNumber = getPieceNumber(pieceType);
    pieceNumber++;

    int leadingZeroes = 3;
    int n = pieceNumber;
    while (n != 0) {
        leadingZeroes--;
        n /= 10;
    }

    _labels->at(pieceType)->setText(
        QString::fromStdString(
            std::to_string(pieceNumber)
            .insert(0, leadingZeroes, '0')));
}

int StatisticsView::getPieceNumber(PieceData::PieceType pieceType) {
    QString previousString = _labels->at(pieceType)->text();
    return std::stoi(previousString.toStdString());
}

void StatisticsView::onPieceLocked() {
    PieceData::PieceType pieceType;
    int rotation = 0;

    CurrentPiece::getInstance()->getNextPieceData(pieceType, rotation);
    this->incrementPieceNumber(pieceType);
}