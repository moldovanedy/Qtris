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
}

UI::NextPieceView::~NextPieceView() {}

void UI::NextPieceView::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(Qt::PenStyle::SolidLine);
    painter.setBrush(QBrush(QColor(0x00, 0xff, 0xff)));
    painter.drawRect(QRect(60, 80, 120, 60));
}