#ifndef GAME_HPP
#define GAME_HPP

#include <QObject>
#include <QTimer>
#include <QGraphicsView>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = 0);
    explicit Game(QGraphicsView *view);

    void run();

signals:
    
public slots:
    void update();

public:
    QTimer* updateTimer;

private:
    QGraphicsView* m_view;
};

#endif // GAME_HPP
