#include "cutie.hpp"

#include <QDebug>


Cutie::Cutie(Game *game, QString path, QPointF pos) :
    Sprite(game, path)
{
    setPos(pos);
}

void Cutie::prePaint(QPainter *painter)
{
    float wobble = fabs(sin(m_lifeTime * 5)) * 0.1;
    painter->translate(0, - m_pixmap.height() * wobble);
    painter->scale(0.3, 0.3 * (1 + wobble));
}


void Cutie::update(float dt)
{
    moveBy(dt*20, 0);
}
