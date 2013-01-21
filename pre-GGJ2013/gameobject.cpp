#include "gameobject.hpp"

#include "game.hpp"

GameObject::GameObject(Game *game) :
    m_game(game)
{
    QObject::connect(m_game->updateTimer, SIGNAL(timeout()), this, SLOT(callUpdate()));
}

void GameObject::callUpdate()
{
    update(m_game->updateTimer->interval() / 1000.0); // TODO
}

void GameObject::update(float dt)
{
}
