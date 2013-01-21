#include "cutie.hpp"

#include <QDebug>

Cutie::Cutie(Game *game) :
    Sprite(game, ":gfx/cutie-0")
{
}

void Cutie::prePaint(QPainter *painter)
{
    float wobble = fabs(sin(m_lifeTime * 5)) * 0.1;
    painter->translate(500, 500 - m_pixmap.height() * wobble);
    painter->scale(0.3, 0.3 * (1 + wobble));
}
