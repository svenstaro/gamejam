#ifndef CUTIE_HPP
#define CUTIE_HPP

#include "sprite.hpp"

class Cutie : public Sprite
{
    Q_OBJECT
public:
    explicit Cutie(Game *game);

    void prePaint(QPainter *painter);

};

#endif // CUTIE_HPP
