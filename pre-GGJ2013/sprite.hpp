#ifndef SRPITE_HPP
#define SRPITE_HPP

#include <QPointF>

#include "gameobject.hpp"

class Sprite : public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF scale READ scale WRITE setScale)

public:
    explicit Sprite(Game *game, QString file);

    virtual void prePaint(QPainter *painter);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual QRectF boundingRect() const;

    QPointF scale() const;
    void setScale(QPointF scale);

protected:
    QPixmap m_pixmap;

private:
    QPointF m_scale;
};

#endif // SRPITE_HPP
