part of game;

class RainDrop extends Sprite {

    RainDrop(randX, randY) {
        randX *= 100;
        randY *= 100;
        var radius = random.nextInt(10);
        graphics.circle(randX, randY, 5 + radius);
        scaleX = 0.01;
        scaleY = 0.01;
        var gradient = new GraphicsGradient.radial(randX, randY, 0, randX, randY, 5 + radius);
        gradient.addColorStop(0, 0x50AAAAAA);
        gradient.addColorStop(1, 0x006666AA);
        this.graphics.fillGradient(gradient);

        var transition = new Transition(1.0, 0.0, 2, TransitionFunction.easeInExponential);
        transition.onUpdate = (value) => this.alpha = value;
        transition.onComplete = () => this.removeFromParent();
        stage.juggler.add(transition);

        mouseEnabled = false;
    }
}
