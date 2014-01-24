part of game;

class HumanEvent extends Sprite {
    
    num time = 0;
    
    HumanEvent() {
        this.onEnterFrame.listen(_update);
        
        this.transformationMatrix.scale(100, 80);
        
        this.graphics.rect(-0.25, 0, 0.5, 0.25);
        
        //this.graphics.fillColor(0xFFFF0000);
        
        var gradient = new GraphicsGradient.radial(0, 0, 0, 0, 0, 0.15);
        gradient.addColorStop(0, 0xFFDA2020);
        gradient.addColorStop(1, 0x00DA2020);
        this.graphics.fillGradient(gradient);
    }
    
    _update(EnterFrameEvent e) {
        time += e.passedTime;
    }
}