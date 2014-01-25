part of game;

class Branch extends Sprite {
    num thickness = 0.1;
    num baseRotation = 0.0;

    num valve = .5;
    bool isDragging = false;
    num startMouseY = 0;

    Shape shape;

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
        // shape.graphics.strokeColor(0xFF00FF00, 0.01);

        shape.onMouseMove.listen(this.dragInProgress);
        shape.onMouseDown.listen(this.dragStart);
        shape.onMouseUp.listen(this.dragStop);
        shape.onMouseOut.listen(this.dragStop);
    }

    void _onEnterFrame(EnterFrameEvent e) {
        // shape.graphics.clear();
        // shape.graphics.rect(-thickness/2, -1, thickness, 1);
        // shape.graphics.strokeColor(0xFF00FF00, 0.01);

        this.rotation = lerp(baseRotation, PI * .5, Wind.power * 0.01);

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

        this.graphics.fillColor(0x44FFFFFF);
        this.graphics.strokeColor(0xFFFFFFFF, 0.01);
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

    void dragStart(MouseEvent event) {
        print("Dragging on depth ${this.getDepth()}");
        isDragging = true;
        startMouseY = mouseY;
    }

    void dragInProgress(MouseEvent event) {
        event.stopPropagation();

        if(isDragging) {
            valve = (valve - (mouseY - startMouseY)).clamp(0, 1);
            startMouseY = mouseY;
        }
    }

    void dragStop(MouseEvent event) {
        isDragging = false;
        startMouseY = 0;
    }
}
