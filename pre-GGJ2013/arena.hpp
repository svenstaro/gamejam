#ifndef ARENA_HPP
#define ARENA_HPP

#include "sprite.hpp"

class Arena : public Sprite
{
    Q_OBJECT
public:
    explicit Arena (Game *game);

    void prePaint(QPainter *painter);

};

#endif // ARENA_HPP
