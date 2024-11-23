#include "render_area.h"

UI::RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    this->setContentsMargins(0, 0, 0, 0);
}

UI::RenderArea::~RenderArea() {}

void UI::RenderArea::redraw()
{
}

void UI::RenderArea::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(Qt::PenStyle::SolidLine);
    painter.setBrush(QBrush(QColor(0x00, 0x00, 0x00)));
    painter.drawRect(QRect(0, 0, this->width(), this->height()));
}