library game;

import 'dart:html' as html;
import 'dart:math';
import 'package:stagexl/stagexl.dart';
import 'package:stagexl_particle/stagexl_particle.dart';
import 'global.dart';

part 'clock.dart';
part 'human_event.dart';

class Branch extends Sprite {
    Branch() {
        this.graphics.rect(-0.1, -1, 0.2, 1);
        this.graphics.circle(0, -1, 0.1);
        this.graphics.fillColor(0xFFFF9900);
        this.graphics.strokeColor(0xFF884400, 0.01);

        this.y = -1;
        this.rotation = 0.0;

        this.onEnterFrame.listen(this._onEnterFrame);
    }

    void _onEnterFrame(EnterFrameEvent e) {
    }
}

void debugTree(int depth, var parent) {
    int count = depth/2 + 1;
    for(int i = 0; i < count; ++i) {
        Branch b = new Branch();
        b.rotation = (i*1.0/(count-1) - 0.5) * 1 * log(depth+1);
        parent.addChild(b);
        if(depth > 1) {
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

    var background = new Shape();
    background.graphics.rect(0, 0, stage.stageWidth, stage.stageHeight);
    background.graphics.fillColor(0xFF133742);
    stage.addChild(background);

    stage.juggler.add(new Clock());

    view = new Sprite();
    view.x = stage.stageWidth / 2;
    view.y = stage.stageHeight - 100;
    view.scaleX = 100;
    view.scaleY = 100;
    stage.addChild(view);

    Branch root = new Branch();
    root.y = 0;
    view.addChild(root);
    debugTree(3, root);

    var particleConfig = {
        "maxParticles":323, "duration":0, "lifeSpan":3.09, "lifespanVariance":0.4, "startSize":10, "startSizeVariance":14, "finishSize":10, "finishSizeVariance":9, "shape":"circle", "emitterType":0, "location":{"x":0, "y":0}, "locationVariance":{"x":100, "y":0}, "speed":100, "speedVariance":52, "angle":90, "angleVariance":0, "gravity":{"x":0, "y":100}, "radialAcceleration":20, "radialAccelerationVariance":0, "tangentialAcceleration":0, "tangentialAccelerationVariance":0, "minRadius":0, "maxRadius":221, "maxRadiusVariance":0, "rotatePerSecond":0, "rotatePerSecondVariance":0, "compositeOperation":"lighter", "startColor":{"red":0.2, "green":0.2, "blue":0.5, "alpha":1}, "finishColor":{"red":0.2, "green":0.2, "blue":1, "alpha":0}
    };

    var ground = new Shape();
    ground.graphics.rect(-10000, 0, 20000, 1000);
    ground.graphics.fillColor(0xFF000000);
    view.addChild(ground);

    var particleEmitter = new ParticleEmitter(particleConfig);
    particleEmitter.setEmitterLocation(200, 200);
    particleEmitter.filters = [new GlowFilter(Color.Yellow, 1.0, 20, 20)];
    stage.addChild(particleEmitter);
    stage.juggler.add(particleEmitter);

    stage.addChild(new HumanEvent());
}
