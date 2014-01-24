import 'dart:html' as html;
import 'dart:math';
import 'package:stagexl/stagexl.dart';
import 'package:dartemis/dartemis.dart';
import 'global.dart';

class Branch extends Sprite {
    Branch() {
        this.graphics.rect(-0.1, -1, 0.2, 1);
        this.graphics.circle(0, -1, 0.1);
        this.graphics.fillColor(0xAAFF9900);
        this.graphics.strokeColor(0xFF884400, 0.01);

        this.y = -1;
        this.rotation = 0.0;

        this.onEnterFrame.listen(this._onEnterFrame);
    }

    void _onEnterFrame(RenderEvent e) {
    }
}

void debugTree(int depth, var parent) {
    int count = 2;
    for(int i = 0; i < count; ++i) {
        Branch b = new Branch();
        b.rotation = (i*1.0/(count-1) - 0.5) * .5 * log(depth+1);
        parent.addChild(b);
        if(depth > 0) {
            debugTree(depth - 1, b);
        }
    }
}

void main() {
    // setup the Stage and RenderLoop
    canvas = html.querySelector('#stage');
    stage = new Stage('myStage', canvas);
    var renderLoop = new RenderLoop();
    renderLoop.addStage(stage);

    view = new Sprite();
    view.x = stage.stageWidth / 2;
    view.y = stage.stageHeight - 100;
    view.scaleX = 100;
    view.scaleY = 100;
    stage.addChild(view);

    debugTree(3, view);

    // stage.pivotX = canvas.width / 2;
    // stage.pivotY = canvas.height - 100;
    // stage.setScale(0.01);

    // var ground = new Shape();
    // ground.rectangle()
}
