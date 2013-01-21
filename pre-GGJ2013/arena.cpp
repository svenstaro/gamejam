#include "arena.hpp"

Arena::Arena(Game *game) :
    Sprite(game, ":gfx/arena")
{
}

void Arena::prePaint(QPainter *painter)
{
    painter->translate(500, 300);
}
