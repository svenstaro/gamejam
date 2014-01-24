part of game;

class Branch extends Sprite {
    num baseRotation = 0.0;

    Branch() {
        this.graphics.rect(-0.1, -1, 0.2, 1);
        this.graphics.circle(0, -1, 0.1);
        this.graphics.fillColor(0xFFFF9900);
        this.graphics.strokeColor(0xFF884400, 0.01);

        this.y = -1;

        this.onEnterFrame.listen(this._onEnterFrame);
    }

    void _onEnterFrame(EnterFrameEvent e) {
        this.rotation = this.baseRotation + (PI * .5 - this.baseRotation) * Wind.power * 0.01;
    }
}
