#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

#include "game.hpp"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QGraphicsScene scene(0, 0, 800, 600);
    scene.setBackgroundBrush(Qt::white);
    Game game;
    scene.addSimpleText("lol");
    QGraphicsView window(&scene);
    window.setFrameStyle(0);
    window.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    window.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    window.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QTimer timer;
    timer.setInterval(16);
    timer.start();
    QObject::connect(&timer, SIGNAL(timeout()), &game, SLOT(update()));

    window.resize(800, 600);
    window.show();

    return application.exec();
}
