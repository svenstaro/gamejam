#include "game.hpp"

Game::Game(QObject *parent) :
    QObject(parent),
    updateTimer(new QTimer())
{
    updateTimer->setInterval(16);
    updateTimer->setSingleShot(false);

    QObject::connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
}

void Game::run() {
    updateTimer->start();
}

void Game::update()
{
    // wut we need this for?
}
