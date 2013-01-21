#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

#include "game.hpp"
#include "cutie.hpp"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QGraphicsScene scene(0, 0, 800, 600);
    scene.setBackgroundBrush(Qt::white);
    scene.addSimpleText("lol");

    QGraphicsView window(&scene);
    window.setFrameStyle(0);
    window.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    window.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    window.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    window.resize(800, 600);
    window.show();

    Game game;
    scene.addItem(new Cutie(&game));
    game.run();

    return application.exec();
}
