#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QGLWidget>

#include "game.hpp"
#include "cutie.hpp"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QGraphicsScene scene(0, 0, 800, 600);
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
    window.resize(800, 600);
    window.setBackgroundBrush(QBrush(Qt::black));
    window.show();

    Game game(&window);
    scene.addItem(new Cutie(&game));
    game.run();

    return application.exec();
}
