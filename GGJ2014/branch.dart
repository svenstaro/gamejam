part of game;

class Branch extends Sprite {
    num water = 0.1;
    num energy = 0.1;

    num thickness = 0.1;

    num baseRotation = 0.0;

    num valve = 0.5;
    bool isDragging = false;
    Vector dragStartPoint = null;

    var branchText = new TextField();

    Shape shape;

    Branch(this.thickness) {
        shape = new Sprite();
        addChild(shape);
        _updateShape();

        this.y = -1;
        onEnterFrame.listen(_onEnterFrame);

        branchText.defaultTextFormat = new TextFormat('Tamsyn', 12, Color.White);
        branchText.scaleX = 0.01;
        branchText.scaleY = 0.01;
        branchText.y = -0.5;
        branchText.text = "branchText";
        shape.addChild(branchText);
    }

    int getDepth() {
        return parent is Branch ? parent.getDepth() + 1 : 0;
    }

    bool isRoot() {
        return !(parent is Branch);
    }

    void growLeaves() {
        if(isRoot()) {
            this.graphics.circle(this.x, this.y, 5);
            this.graphics.fillColor(Color.Green);
            print("lol");
        }

        for(int i = 0; i < numChildren; i++) {
            if(getChildAt(i) is Branch) getChildAt(i).growLeaves();
        }
    }

    void _updateShape() {
        shape.graphics.clear();
        shape.graphics.rect(-0.1, -1, 0.2, 1);
        shape.graphics.fillColor(0);
        shape.graphics.strokeColor(0x0000FF00, 0.001);

        shape.onMouseMove.listen(this.dragInProgress);
        shape.onMouseDown.listen(this.dragStart);
        shape.onMouseUp.listen(this.dragStop);
        shape.onMouseOut.listen(this.dragStop);
    }

    void _onEnterFrame(EnterFrameEvent e) {
        // Update debug info
        branchText.text = "D${getDepth()}";
        branchText.text += "\nW${water}E${energy}";

        this.rotation = lerp(baseRotation, PI * .5, Wind.power * 0.01);

        num st = getStartThickness();
        num et = thickness;

        this.graphics.clear();

        if(isRoot()) {
            List<Point> points = new List<Point>();
            addPoints(points, this);

            this.graphics.beginPath();
            this.graphics.moveTo(points[0].x, points[0].y);
            for(var point in points) {
                this.graphics.lineTo(point.x, point.y);
            }
            this.graphics.closePath();
        }

        this.graphics.fillColor(0x44FFFFFF);
        // this.graphics.strokeColor(0xFFFFFFFF, 0.01);

        this.graphics.fillColor(0xFF000000);
        this.graphics.strokeColor(0, 0);
    }

    void addPoints(List<Point> points, Branch root) {
        num st = getStartThickness();
        num et = thickness;

        // going up on the left
        if(identical(root, this)) {
            points.add(root.globalToLocal(localToGlobal(new Point(-st/2,  0))));
        }
        points.add(root.globalToLocal(localToGlobal(new Point(-et/2, -1))));

        int numBranches = 0;
        for(int i = 0; i < numChildren; i++) {
            if(getChildAt(i) is Branch) numBranches++;
        }

        int branchNumber = 0;
        for(int i = 0; i < numChildren; i++) {
            var child = getChildAt(i);
            if(branchNumber > 0) {
                points.add(root.globalToLocal(localToGlobal(new Point(et*(branchNumber*1.0/numBranches - 0.5), -1.2))));
            }
            if(child is Branch) {
                child.addPoints(points, root);
                branchNumber++;
            }
        }

        // going down on the right
        points.add(root.globalToLocal(localToGlobal(new Point( et/2, -1))));
        if(identical(root, this)) {
            points.add(root.globalToLocal(localToGlobal(new Point( st/2,  0))));
        }
    }

    num getStartThickness() {
        return isRoot() ? thickness : parent.thickness;
    }

    num getAbsoluteAngle() {
        return isRoot() ? rotation : parent.rotation + rotation;
    }

    void dragStart(MouseEvent event) {
        isDragging = true;
        dragStartPoint = new Point(mouseX, mouseY);
    }

    void dragInProgress(MouseEvent event) {
        event.stopPropagation();

        if(isDragging) {
            if(mode == "valve") {
                valve = (valve - (mouseY - dragStartPoint.y)).clamp(0, 1);
                dragStartPoint = new Vector(mouseX, mouseY);
            }
        }
    }

    void dragStop(MouseEvent event) {
        isDragging = false;

        print("Drag stop");

        if(mode == "branch") {
            var mouse = new Vector(mouseX, mouseY);
            num angle = mouse.rads;

            Branch b = new Branch(thickness);
            b.rotation = angle - getAbsoluteAngle();
            addChild(b);
        }
    }

    Vector getTipPosition() {
        var p = view.globalToLocal(localToGlobal(new Point(0, 0)));
        return new Vector(p.x, p.y);
    }
}
