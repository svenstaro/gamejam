#include "cutie.hpp"

Cutie::Cutie(Game *game) :
    GameObject(game),
    m_pixmap(":gfx/cutie-0")
{
}

void Cutie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->scale(0.3, 0.3);
    painter->drawPixmap(100, 100, m_pixmap);
}

QRectF Cutie::boundingRect() const
{
}
