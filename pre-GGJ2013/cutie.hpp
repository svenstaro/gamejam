#ifndef CUTIE_HPP
#define CUTIE_HPP

#include "sprite.hpp"

class Cutie : public Sprite
{
    Q_OBJECT
public:
    explicit Cutie(Game *game, QString path, QPointF pos);

    void prePaint(QPainter *painter);

    void update(float dt);

private:
    QPointF m_target;

};

#endif // CUTIE_HPP
