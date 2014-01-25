part of game;

class Branch extends Sprite {
    num thickness = 0.1;
    num baseRotation = 0.0;

    num valve = .5;
    bool isDragging = false;
    Vector dragStartPoint = null;

    Sprite shape;

    Branch() {
        shape = new Sprite();
        addChild(shape);
        _updateShape();

        this.y = -1;
        onEnterFrame.listen(_onEnterFrame);
    }

    int getDepth() {
        return parent is Branch ? parent.getDepth() + 1 : 0;
    }

    bool isRoot() {
        return !(parent is Branch);
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
        // shape.graphics.clear();
        // shape.graphics.rect(-thickness/2, -1, thickness, 1);
        // shape.graphics.strokeColor(0xFF00FF00, 0.01);

        this.rotation = lerp(baseRotation, PI * .5, Wind.power * 0.003);

        num st = getStartThickness();
        num et = thickness;

        this.graphics.clear();
        // this.graphics.moveTo(-et/2, -1);
        // this.graphics.lineTo(-st/2, 0);
        // this.graphics.lineTo( st/2, 0);
        // this.graphics.lineTo( et/2, -1);
        // this.graphics.closePath();

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

        //this.graphics.fillColor(0x44FFFFFF);
        // this.graphics.strokeColor(0xFFFFFFFF, 0.01);
        //this.graphics.fillColor(0xFF000000);
        
        this.graphics.fillColor(Environment.getLightColorFor(this).hex);
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

        // sort children
        // sortChildren((var l, var r) {
        //     if(l is Branch && r is Branch) {
        //         return l.baseRotation < r.baseRotation;
        //     } else {
        //         return 1;
        //     }
        // });

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

            Branch b = new Branch();
            b.rotation = angle - getAbsoluteAngle();
            b.thickness = thickness;
            addChild(b);
        }
    }

    Vector getTipPosition() {
        var p = view.globalToLocal(localToGlobal(new Point(0, 0)));
        return new Vector(p.x, p.y);
    }
}
