library game;

import 'dart:html' as html;
import 'dart:math';
import 'package:stagexl/stagexl.dart';
import 'package:stagexl_particle/stagexl_particle.dart';
import 'package:noise/noise.dart';
import 'global.dart';

part 'clock.dart';
part 'human_event.dart';
part 'wind.dart';
part 'raindrop.dart';
part 'branch.dart';
part 'debug.dart';

Shape makeGround(double seed) {
    var gen = makeOctave2(simplex2, 3, 0.01);

    var shape = new Shape();
    var ref = gen(seed, 0);

    shape.graphics.beginPath();
    shape.graphics.moveTo(-100, 100);
    for(num x = -100; x < 100; x += 0.1) {
        var y = gen(seed, x * 0.2) - ref;
        shape.graphics.lineTo(x, y * 0.2);
    }
    shape.graphics.lineTo(100, 100);
    shape.graphics.lineTo(-100, 100);
    shape.graphics.closePath();

    shape.graphics.fillColor(0xFF000000);
    return shape;
}

void main() {
    random = new Random();

    // setup the Stage and RenderLoop
    canvas = html.querySelector('#stage');
    stage = new Stage('stage', canvas);
    var renderLoop = new RenderLoop();
    renderLoop.addStage(stage);

    var background = new Shape();
    background.name = "background";
    background.graphics.rect(0, 0, stage.stageWidth, stage.stageHeight);
    background.graphics.fillColor(0xFF133742);
    stage.addChild(background);

    stage.juggler.add(new Clock());
    stage.juggler.add(new Wind());

    view = new Sprite();
    view.name = "view";
    view.x = stage.stageWidth / 2;
    view.y = stage.stageHeight - 100;
    view.scaleX = 100;
    view.scaleY = 100;

    // Rain
    view.onEnterFrame.listen((e) {
        var amountOfRain = 2;

        for (var i = 0; i < amountOfRain; i++) {
            var randX = random.nextInt(stage.stageWidth);
            var randY = random.nextInt(stage.stageHeight);
            var obj = stage.hitTestInput(randX, randY);
            if(["view", "ground", "branch"].contains(obj.name)) {
                var raindrop = new RainDrop(randX, randY);
                stage.addChild(raindrop);
            }
        }
    });
    stage.addChild(view);

    Branch root = new Branch();
    root.y = 0;
    view.addChild(root);
    debugTree(3, root);
    root.growLeaves();

    var particleConfig = {
        "maxParticles":323, "duration":0, "lifeSpan":3.09, "lifespanVariance":0.4, "startSize":10, "startSizeVariance":14, "finishSize":10, "finishSizeVariance":9, "shape":"circle", "emitterType":0, "location":{"x":0, "y":0}, "locationVariance":{"x":100, "y":0}, "speed":100, "speedVariance":52, "angle":90, "angleVariance":0, "gravity":{"x":0, "y":100}, "radialAcceleration":20, "radialAccelerationVariance":0, "tangentialAcceleration":0, "tangentialAccelerationVariance":0, "minRadius":0, "maxRadius":221, "maxRadiusVariance":0, "rotatePerSecond":0, "rotatePerSecondVariance":0, "compositeOperation":"lighter", "startColor":{"red":0.2, "green":0.2, "blue":0.5, "alpha":1}, "finishColor":{"red":0.2, "green":0.2, "blue":1, "alpha":0}
    };

    var ground = makeGround(random.nextDouble() * 100);
    view.addChild(ground);

    var particleEmitter = new ParticleEmitter(particleConfig);
    particleEmitter.setEmitterLocation(200, 200);
    particleEmitter.filters = [new GlowFilter(Color.Yellow, 1.0, 20, 20)];
    /*stage.addChild(particleEmitter);*/
    /*stage.juggler.add(particleEmitter);*/

    stage.addChild(new HumanEvent());
}
