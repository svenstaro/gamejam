part of game;

class RainDrop extends Sprite {
    
    num time = 0;
    
    RainDrop(rand_x, rand_y) {
        this.onEnterFrame.listen(_update);
        
        var radius = random.nextInt(10);
        this.graphics.circle(rand_x, rand_y, radius);
        var gradient = new GraphicsGradient.radial(rand_x, rand_y, 0, rand_x, rand_y, radius);
        gradient.addColorStop(0, 0x506666AA);
        gradient.addColorStop(1, 0x006666AA);
        this.graphics.fillGradient(gradient);

        stage.juggler.transition(1.0, 0.0, 1, TransitionFunction.linear, (value) => this.alpha = value);
    }
    
    _update(EnterFrameEvent e) {
        time += e.passedTime;
    }
}
