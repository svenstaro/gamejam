#include "cutie.hpp"

Cutie::Cutie(Game *game) :
    Sprite(game, ":gfx/cutie-0")
{
}

void Cutie::prePaint(QPainter *painter)
{
    painter->translate(500, 500);
    painter->scale(0.3, 0.3);
}
