#include "game.hpp"

Game::Game(QObject *parent)
    : QObject(parent)
{
}

Game::Game(QGraphicsView *view) :
    QObject(0),
    updateTimer(new QTimer()),
    m_view(view)
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
    m_view->viewport()->update();
}
