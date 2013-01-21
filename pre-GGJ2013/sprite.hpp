#ifndef SRPITE_HPP
#define SRPITE_HPP

#include "gameobject.hpp"

class Sprite : public GameObject
{
public:
    explicit Sprite(Game *game, QString file);

    virtual void prePaint(QPainter *painter);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual QRectF boundingRect() const;

protected:
    QPixmap m_pixmap;

};

#endif // SRPITE_HPP
