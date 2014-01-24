import 'dart:html' as html;
import 'package:stagexl/stagexl.dart';
import 'global.dart';
import 'clock.dart';

void main() {
    // setup the Stage and RenderLoop
    canvas = html.querySelector('#stage');
    stage = new Stage('myStage', canvas);
    var renderLoop = new RenderLoop();
    renderLoop.addStage(stage);
    
    stage.addChild(new Clock());

    // draw a red circle
    var parent = new Sprite();
    var shape = new Shape();
    shape.graphics.circle(100, 100, 60);
    shape.graphics.fillColor(Color.Red);

    
    parent.onEnterFrame.listen((EnterFrameEvent e){
        //move faster the later the current daytime is
        parent.transformationMatrix.translate(Clock.daytime * e.passedTime * 20, 0);
    });

    shape.addTo(parent);
    stage.addChild(parent);
}
