#ifndef CUTIE_HPP
#define CUTIE_HPP

#include "gameobject.hpp"

class Cutie : public GameObject
{
    Q_OBJECT
public:
    explicit Cutie(Game *game);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual QRectF boundingRect() const;

signals:
    
public slots:

private:
    QPixmap m_pixmap;

};

#endif // CUTIE_HPP
