#ifndef GAME_HPP
#define GAME_HPP

#include <QObject>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = 0);
    
signals:
    
public slots:
    void update();
    
};

#endif // GAME_HPP
