#include "sprite.hpp"

Sprite::Sprite(Game *game, QString file) :
    GameObject(game),
    m_pixmap(file)
{
}

void Sprite::prePaint(QPainter *painter)
{
}

void Sprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    prePaint(painter);
    painter->drawPixmap(-m_pixmap.width() / 2, -m_pixmap.height() / 2, m_pixmap);
}

QRectF Sprite::boundingRect() const
{
    return m_pixmap.rect();
}
