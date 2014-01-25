library game;

import 'dart:html' as html;
import 'dart:math';
import 'package:stagexl/stagexl.dart';
import 'package:stagexl_particle/stagexl_particle.dart';
import 'package:noise/noise.dart';
import 'global.dart';

part 'math.dart';
part 'awesome_color.dart';
part 'clock.dart';
part 'environment.dart';
part 'spline.dart';
part 'human_event.dart';
part 'leaf.dart';
part 'wind.dart';
part 'raindrop.dart';
part 'branch.dart';
part 'debug.dart';

Shape makeGround(double seed) {
    var gen = makeOctave2(simplex2, 3, 0.01);

    var shape = new Shape();
    var ref = gen(seed, 0.0);

    amountOfRain = 2;

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
    resourceManager = new ResourceManager()
        ..addSound('noise','data/noise.ogg');

    resourceManager.load().then((_) {
        run();
    }).catchError((e) => print(e));
}

void run() {
    random = new Random();

    // setup the Stage and RenderLoop
    canvas = html.querySelector('#stage');
    stage = new Stage(canvas);
    stage.focus = stage;
    var renderLoop = new RenderLoop();
    renderLoop.addStage(stage);

    var background = new Shape();
    background.graphics.rect(0, 0, stage.stageWidth, stage.stageHeight);
    stage.addChild(background);

    if(relaxMode) {
        var g = new GraphicsGradient.linear(0, 0, 0, stage.stageHeight);
        g.addColorStop(0, 0xFF446688);
        g.addColorStop(1, 0xFFAACCFF);
        background.graphics.fillGradient(g);
    } else {
        background.graphics.fillColor(0xFF000000);
    }

    stage.juggler.add(new Environment());
    stage.juggler.add(new Clock());
    stage.juggler.add(new Wind());
    Wind.windPower = 0.07;
    Wind.secondsPerWave = 9.2;

    view = new Sprite();
    view.x = stage.stageWidth / 2;
    view.y = stage.stageHeight - 100;
    view.scaleX = 100;
    view.scaleY = 100;

    stage.addChild(view);

    Branch root = new Branch(0.6);
    root.y = 0;
    view.addChild(root);

    /*debugTree(0, root);*/
    retardTree(0, root);

    Branch rootRoot = new Branch(0.6);
    rootRoot.y = 0;
    rootRoot.isRoot = true;
    rootRoot.baseRotation = PI;
    rootRoot.length = 0.1;
    view.addChild(rootRoot);
    debugRoots(0, rootRoot);

    var particleConfig = {
        "maxParticles":323, "duration":0, "lifeSpan":3.09, "lifespanVariance":0.4, "startSize":10, "startSizeVariance":14, "finishSize":10, "finishSizeVariance":9, "shape":"circle", "emitterType":0, "location":{"x":0, "y":0}, "locationVariance":{"x":100, "y":0}, "speed":100, "speedVariance":52, "angle":90, "angleVariance":0, "gravity":{"x":0, "y":100}, "radialAcceleration":20, "radialAccelerationVariance":0, "tangentialAcceleration":0, "tangentialAccelerationVariance":0, "minRadius":0, "maxRadius":221, "maxRadiusVariance":0, "rotatePerSecond":0, "rotatePerSecondVariance":0, "compositeOperation":"lighter", "startColor":{"red":0.2, "green":0.2, "blue":0.5, "alpha":1}, "finishColor":{"red":0.2, "green":0.2, "blue":1, "alpha":0}
    };

    var ground = makeGround(random.nextDouble() * 100.0);
    // view.addChild(ground);

    var particleEmitter = new ParticleEmitter(particleConfig);
    particleEmitter.setEmitterLocation(200, 200);
    //particleEmitter.filters = [new GlowFilter(Color.Yellow, 1.0, 20, 20)];
    /*stage.addChild(particleEmitter);*/
    /*stage.juggler.add(particleEmitter);*/

    view.addChild(new HumanEvent());

    // Rain
    view.onEnterFrame.listen((e) {
        for (var i = 0; i < amountOfRain; i++) {
            var randX = random.nextInt(stage.stageWidth);
            var randY = random.nextInt(stage.stageHeight);
            var obj = stage.hitTestInput(randX, randY);
            if(obj is GlassPlate || identical(obj, ground)) {
                var raindrop = new RainDrop(randX, randY);
                if(!debug) {
                    stage.addChild(raindrop);
                }
            }
        }
    });

    debugText = new TextField();
    debugText.defaultTextFormat = new TextFormat('monospace', 10, Color.White);
    debugText.text = "Debug text";
    debugText.x = 10;
    debugText.y = 10;
    debugText.width = 300;
    stage.addChild(debugText);

    mode = "valve";

    debugShape = new Sprite();
    view.addChild(debugShape);

    var currentBranch = null;
    stage.onMouseDown.listen((MouseEvent e) {
        var p = root.globalToLocal(new Point(e.stageX, e.stageY));
        var pv = view.globalToLocal(new Point(e.stageX, e.stageY));
        var obj = root.hitTestInput(p.x, p.y);
        if(obj is GlassPlate && mode == "valve") {
            obj = obj.parent;

            obj.dragStart(e);
            currentBranch = obj;
        } else if(obj is Branch && mode == "branch") {
            debugShape.graphics.clear();
            debugShape.graphics.circle(pv.x, pv.y, 0.1);
            debugShape.graphics.fillColor(0xAA00FF00);

            obj.growChild(1);
        }
    });

    stage.onMouseMove.listen((MouseEvent e) {
        if(currentBranch is Branch) {
            currentBranch.dragInProgress(e);
        }
    });

    stage.onMouseUp.listen((MouseEvent e) {
        if(currentBranch is Branch) {
            currentBranch.dragStop(e);
        }
        currentBranch = null;
    });

    stage.onKeyDown.listen((e) {
        if(e.keyCode == 68) {
            debug = !debug;
        }
    });

    var sound = resourceManager.getSound('noise');
    var t = sound.play(true);

    view.onEnterFrame.listen((e) {
        if(debug) {
            t.soundTransform = new SoundTransform.mute();
        } else {
            t.soundTransform = new SoundTransform(pow(Wind.windPower, 1.5) * 2, 0);
        }
        debugMessage = Wind.power;

        num mx = stage.mouseX;
        num my = stage.mouseY;
        debugText.text = "Mode: $mode";
        debugText.text += "\nFPS: ${(1.0 / e.passedTime).round()}";
        debugText.text += "\nUnder mouse: ${stage.hitTestInput(mx, my)}";
        debugText.text += "\nMouse Pos: ${mx.round()} / ${my.round()}";
        if(debugMessage != "") debugText.text += "\nDebug message: ${debugMessage}";

        debugText.visible = debug;
    });

}
