#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QtGui>

class Game;

class GameObject : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    GameObject(Game *game);
    // virtual ~GameObject() = 0;

    virtual void update(float dt);

private slots:
    void callUpdate();

protected:
    Game* m_game;
    float m_lifeTime;

};

#endif // GAMEOBJECT_H
