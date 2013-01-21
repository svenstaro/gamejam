#ifndef GAME_HPP
#define GAME_HPP

#include <QObject>
#include <QTimer>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = 0);

    void run();

signals:
    
public slots:
    void update();

public:
    QTimer* updateTimer;

};

#endif // GAME_HPP
