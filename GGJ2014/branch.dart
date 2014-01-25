part of game;

class Branch extends Sprite {
    num baseRotation = 0.0;

    num valve = .5;
    bool isDragging = false;
    num startMouseY = 0;

    Branch() {
        this.name = "branch";
        this.graphics.rect(-0.1, -1, 0.2, 1);
        this.graphics.circle(0, -1, 0.1);
        this.graphics.fillColor(0xFF222222);
        this.graphics.strokeColor(0xFF111111, 0.01);

        this.y = -1;

        this.onEnterFrame.listen(this._onEnterFrame);

        this.onMouseMove.listen(this.dragInProgress);
        this.onMouseDown.listen(this.dragStart);
        this.onMouseUp.listen(this.dragStop);
        this.onMouseOut.listen(this.dragStop);
    }

    void growLeaves() {
        for (var i = 0; i < this.numChildren; i++) {
            this.getChildAt(i).growLeaves();
        }

        if(this.numChildren == 0) {
            this.graphics.circle(0, 0, 0.1);
            this.graphics.fillColor(Color.Green);
            print("lol");
        }
    }

    void _onEnterFrame(EnterFrameEvent e) {
        this.rotation = this.baseRotation + (PI * .5 - this.baseRotation) * Wind.power * 0.01;
    }

    void dragStart(MouseEvent event) { 
        this.isDragging = true; 
        this.startMouseY = this.mouseY; 
    }

    void dragInProgress(MouseEvent event) {
        event.stopPropagation();

        if(this.isDragging) {
            this.valve -= (this.mouseY - this.startMouseY);
            this.startMouseY = this.mouseY;

            this.valve = (
                (this.valve >= 1) ? 1 : (
                    (this.valve <= 0) ? 0 : this.valve
                )
            );
        }
    }

    void dragStop(MouseEvent event) {
        this.isDragging = false; 
        this.startMouseY = 0;
    }
}
