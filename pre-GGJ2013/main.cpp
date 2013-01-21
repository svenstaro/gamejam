#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QGLWidget>

#include "game.hpp"
#include "cutie.hpp"
#include "arena.hpp"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QGraphicsScene scene(0, 0, 1000, 600);
    scene.setBackgroundBrush(Qt::white);

    QGraphicsView window(&scene);
    window.setFrameStyle(0);
    window.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    window.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    window.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    window.setViewport(new QGLWidget());
    //window.setRenderHint(QPainter::Antialiasing, true);
    //window.setRenderHint(QPainter::HighQualityAntialiasing, true);
    window.setRenderHints(QPainter::SmoothPixmapTransform);
    window.resize(1000, 600);
    window.setBackgroundBrush(QBrush(Qt::black));
    window.show();

    Game game(&window);
    scene.addItem(new Arena(&game));
    scene.addItem(new Cutie(&game, ":gfx/cutie-0"));
    game.run();

    return application.exec();
}
