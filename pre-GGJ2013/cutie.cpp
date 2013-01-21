#include "cutie.hpp"

Cutie::Cutie(Game *game) :
    GameObject(game),
    m_pixmap(":gfx/cutie-0")
{
}

void Cutie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->scale(0.1, 0.1);
    painter->drawPixmap(100, 100, m_pixmap);
    painter->drawLine(0, 0, 1000, 1000);
}

QRectF Cutie::boundingRect() const
{
}
