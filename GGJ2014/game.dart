library game;

import 'dart:html' as html;
import 'dart:math';
import 'package:stagexl/stagexl.dart';
import 'packages/stagexl_particle/stagexl_particle.dart';
import 'package:noise/noise.dart';
import 'global.dart';

part 'math.dart';
part 'awesome_color.dart';
part 'clock.dart';
part 'environment.dart';
part 'spline.dart';
part 'human_event.dart';
part 'leaf.dart';
part 'pulse.dart';
part 'wind.dart';
part 'raindrop.dart';
part 'branch.dart';
part 'eye_toggle.dart';
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

void updateBackground() {
    background.graphics.clear();
    background.graphics.rect(0, 0, stage.stageWidth, stage.stageHeight);

    if(relaxMode) {
        var g = new GraphicsGradient.linear(0, 0, 0, stage.stageHeight);
        g.addColorStop(0, 0xFF446688);
        g.addColorStop(1, 0xFFAACCFF);
        background.graphics.fillGradient(g);
    } else {
        background.graphics.fillColor(0xFF000000);
    }
}

void updateRelaxMode() {
    updateBackground();
}

void run() {
    random = new Random();

    // setup the Stage and RenderLoop
    canvas = html.querySelector('#stage');
    stage = new Stage(canvas);
    stage.focus = stage;
    stage.scaleMode = StageScaleMode.NO_SCALE;
    stage.align = StageAlign.TOP_LEFT;
    stage.onResize.listen((e) {
        updateBackground();
        view.x = stage.stageWidth * 0.5;
        view.y = stage.stageHeight * 0.7;
    });
    var renderLoop = new RenderLoop();
    renderLoop.addStage(stage);

    background = new Shape();
    stage.addChild(background);
    updateBackground();

    stage.juggler.add(new Environment());
    stage.juggler.add(new Clock());
    stage.juggler.add(new Wind());
    Wind.windPower = 0.07;
    Wind.secondsPerWave = 9.2;

    view = new Sprite();
    view.x = stage.stageWidth * 0.5;
    view.y = stage.stageHeight * 0.7;
    view.scaleX = 100;
    view.scaleY = 100;
    stage.addChild(view);

    var ground = makeGround(random.nextDouble() * 100.0);
    view.addChild(ground);

    treeBase = new Branch(0.4);
    treeBase.y = 0;
    view.addChild(treeBase);
    if(debug)
        debugTree(0, treeBase);
    else
        gameTree(0, treeBase);
        

    rootBase = new Branch(0.4);
    rootBase.y = 0;
    rootBase.isRoot = true;
    rootBase.baseRotation = PI;
    rootBase.length = 0.1;
    view.addChild(rootBase);
    debugRoots(0, rootBase);

    gameText = new TextField();
    gameText.defaultTextFormat = new TextFormat('monospace', 24, Color.White);
    gameText.width = 300;
    gameText.autoSize = "CENTER";
    gameText.x = stage.stageWidth / 2 - gameText.width / 2;
    gameText.y = stage.stageHeight / 2;
    gameText.text = "Your tree died. :(";
    gameText.mouseEnabled = false;
    gameText.visible = false;
    stage.addChild(gameText);

    // Death
    view.onEnterFrame.listen((e) {
        if(!canDie) return;
        if(treeBase.branches.every((e) => e.isEndBranch) && !isDead) {
            gameText.visible = true;
            isDead = true;

            stage.juggler.transition(1, 0, 10, TransitionFunction.linear, (value) => view.alpha = value);
            stage.juggler.transition(0, 1, 20, TransitionFunction.linear, (value) => gameText.alpha = value);
        }
    });

    var particleConfig = {
        "maxParticles":323, "duration":0, "lifeSpan":3.09, "lifespanVariance":0.4, "startSize":10, "startSizeVariance":14, "finishSize":10, "finishSizeVariance":9, "shape":"circle", "emitterType":0, "location":{"x":0, "y":0}, "locationVariance":{"x":100, "y":0}, "speed":100, "speedVariance":52, "angle":90, "angleVariance":0, "gravity":{"x":0, "y":100}, "radialAcceleration":20, "radialAccelerationVariance":0, "tangentialAcceleration":0, "tangentialAccelerationVariance":0, "minRadius":0, "maxRadius":221, "maxRadiusVariance":0, "rotatePerSecond":0, "rotatePerSecondVariance":0, "compositeOperation":"lighter", "startColor":{"red":0.2, "green":0.2, "blue":0.5, "alpha":1}, "finishColor":{"red":0.2, "green":0.2, "blue":1, "alpha":0}
    };

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
            var p = view.globalToLocal(new Point(randX, randY));
            if(obj is GlassPlate || identical(obj, ground)) {
                var raindrop = new RainDrop(p.x, p.y);
                if(!debug) {
                    view.addChild(raindrop);
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
        var p = treeBase.globalToLocal(new Point(e.stageX, e.stageY));
        var pv = view.globalToLocal(new Point(e.stageX, e.stageY));
        var obj = treeBase.hitTestInput(p.x, p.y);
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

    stage.onMouseRightDown.listen((MouseEvent e) {
        var p = treeBase.globalToLocal(new Point(e.stageX, e.stageY));
        var obj = treeBase.hitTestInput(p.x, p.y);
        if(obj is GlassPlate) {
            obj = obj.parent;
            obj.deleteSoon = true;
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
        debugMessage = "${Wind.power}";

        num mx = stage.mouseX;
        num my = stage.mouseY;
        debugText.text = "Mode: $mode";
        debugText.text += "\nFPS: ${(1.0 / e.passedTime).round()}";
        debugText.text += "\nUnder mouse: ${stage.hitTestInput(mx, my)}";
        debugText.text += "\nMouse Pos: ${mx.round()} / ${my.round()}";
        if(debugMessage != "") debugText.text += "\nDebug message: ${debugMessage}";

        debugText.visible = debug;
    });

    var b = rootBase;
    while(!b.isEndBranch) b = b.branches[0];

    stage.addChild(new EyeToggle());
}
