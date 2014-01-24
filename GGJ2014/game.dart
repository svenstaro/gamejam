import 'dart:html' as html;
import 'package:stagexl/stagexl.dart';
import 'package:stagexl_particle/stagexl_particle.dart';
import 'global.dart';
import 'clock.dart';

class Branch extends Sprite {
    Branch() {
        // this.graphics.beginPath();

        // this.graphics.

        // this.graphics.closePath();
        this.graphics.circle(100, 100, 20);
        this.graphics.fillColor(0xAA000000);
        this.graphics.strokeColor(0xFF000000, 2);
    }
}

void main() {
    // setup the Stage and RenderLoop
    canvas = html.querySelector('#stage');
    stage = new Stage('myStage', canvas);
    var renderLoop = new RenderLoop();
    renderLoop.addStage(stage);
    
    stage.juggler.add(new Clock());

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

    var b = new Branch();
    stage.addChild(b);

    var particleConfig = 
      {"maxParticles":323, "duration":0, "lifeSpan":3.09, "lifespanVariance":0.4, "startSize":10, "startSizeVariance":14, "finishSize":10, "finishSizeVariance":9, "shape":"circle", "emitterType":0, "location":{"x":0, "y":0}, "locationVariance":{"x":100, "y":0}, "speed":100, "speedVariance":52, "angle":90, "angleVariance":0, "gravity":{"x":0, "y":100}, "radialAcceleration":20, "radialAccelerationVariance":0, "tangentialAcceleration":0, "tangentialAccelerationVariance":0, "minRadius":0, "maxRadius":221, "maxRadiusVariance":0, "rotatePerSecond":0, "rotatePerSecondVariance":0, "compositeOperation":"lighter", "startColor":{"red":0.2, "green":0.2, "blue":0.5, "alpha":1}, "finishColor":{"red":0.2, "green":0.2, "blue":1, "alpha":0}
      };

    var particleEmitter = new ParticleEmitter(particleConfig);
    particleEmitter.setEmitterLocation(200, 200);
    particleEmitter.filters = [new GlowFilter(Color.Yellow, 1.0, 20, 20)];
    stage.addChild(particleEmitter);
    stage.juggler.add(particleEmitter);
}
