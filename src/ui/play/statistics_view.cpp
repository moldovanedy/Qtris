#include "statistics_view.h"

UI::StatisticsView::StatisticsView(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(QTRIS_DATA_AREA_STYLE);
    this->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    this->setContentsMargins(0, 0, 0, 0);
    this->setMaximumWidth(300);

    QBoxLayout *mainColumn = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);

    QLabel *title = new QLabel(this);
    title->setStyleSheet("border: none");
    title->setFont(PlayArea::getDataPixelFont());
    title->setText("STATISTICS");
    title->setContentsMargins(0, 0, 0, 0);
    title->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum);
    title->setMaximumWidth(300);

    mainColumn->addWidget(title);

    for (int i = 0; i < 7; i++) {
        DataManager::PieceData::PieceType pieceType;
        switch (i)
        {
        default:
        case 0:
            pieceType = DataManager::PieceData::PieceType::T;
            break;
        case 1:
            pieceType = DataManager::PieceData::PieceType::J;
            break;
        case 2:
            pieceType = DataManager::PieceData::PieceType::Z;
            break;
        case 3:
            pieceType = DataManager::PieceData::PieceType::O;
            break;
        case 4:
            pieceType = DataManager::PieceData::PieceType::S;
            break;
        case 5:
            pieceType = DataManager::PieceData::PieceType::L;
            break;
        case 6:
            pieceType = DataManager::PieceData::PieceType::I;
            break;
        }

        QBoxLayout *row = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
        row->setAlignment(Qt::AlignCenter);

        QPoint spawnPoint = QPoint();
        QImage *piece = UI::Resources::getPieceImage(pieceType, -1, spawnPoint);
        QImage scaledPiece;
        if (pieceType == DataManager::PieceData::PieceType::I) {
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
        row->addWidget(imageView);

        QLabel *statLabel = new QLabel(this);
        statLabel->setText("000");
        statLabel->setContentsMargins(0, 0, 0, 0);
        statLabel->setFont(PlayArea::getDataPixelFont());
        statLabel->setStyleSheet("border: none");

        row->addWidget(statLabel);
        this->_labels->insert(std::make_pair(pieceType, statLabel));

        mainColumn->addLayout(row);
    }
}

UI::StatisticsView::~StatisticsView() {}

void UI::StatisticsView::incrementPieceNumber(DataManager::PieceData::PieceType pieceType) {
    int pieceNumber = getPieceNumber(pieceType);
    pieceNumber++;
    this->_labels->at(pieceType)->setText(QString(std::to_string(pieceNumber).c_str()));
}

int UI::StatisticsView::getPieceNumber(DataManager::PieceData::PieceType pieceType) {
    QString previousString = this->_labels->at(pieceType)->text();
    return std::stoi(previousString.toStdString());
}