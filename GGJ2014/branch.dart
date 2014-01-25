part of game;

class Branch extends Sprite {
    num thickness = 0.1;
    num baseRotation = 0.0;

    num valve = .5;
    bool isDragging = false;
    Vector dragStartPoint = null;

    var shape;

    Branch() {
        shape = new GlassPlate(thickness, 1);
        shape.pivotX = thickness/2;
        shape.pivotY = 1;
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

    bool isEndBranch() {
        for(int i = 0; i < numChildren; i++) {
            if(getChildAt(i) is Branch) return false;
        }
        return true;
    }

    void _updateShape() {
        shape.onMouseMove.listen(this.dragInProgress);
        shape.onMouseDown.listen(this.dragStart);
        shape.onMouseUp.listen(this.dragStop);
        shape.onMouseOut.listen(this.dragStop);
    }

    void _onEnterFrame(EnterFrameEvent e) {
        shape.width = thickness;
        shape.pivotX = thickness/2;
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
            Spline spline = new Spline();
            addPoints(spline, this);
            spline.generatePath(this.graphics);
        }

        this.graphics.fillColor(0x44FFFFFF);
        // this.graphics.strokeColor(0xFFFFFFFF, 0.01);

        this.graphics.fillColor(0xFF000000);
        this.graphics.strokeColor(0, 0);
    }

    void addPoints(Spline spline, Branch root) {
        num st = getStartThickness();
        num et = thickness;

        num tangentLength = isEndBranch() ? 0.0 : 0.3;

        // going up on the left
        if(isRoot()) {
            spline.add(root.globalToLocal(localToGlobal(new Point(-st/2 * 1.5, 0.2))), 0.1);
            spline.add(root.globalToLocal(localToGlobal(new Point(-st/2,  0))), 0.1);
        }
        spline.add(root.globalToLocal(localToGlobal(new Point(-et/2, -1))), tangentLength);

        // sort children
        sortChildren((var l, var r) {
            if(l is Branch && r is Branch) {
                return l.baseRotation.compareTo(r.baseRotation);
            } else {
                return 0;
            }
        });

        int numBranches = 0;
        for(int i = 0; i < numChildren; i++) {
            if(getChildAt(i) is Branch) numBranches++;
        }

        int branchNumber = 0;
        for(int i = 0; i < numChildren; i++) {
            var child = getChildAt(i);
            if(branchNumber > 0) {
                spline.add(root.globalToLocal(localToGlobal(new Point(et*(branchNumber*1.0/numBranches - 0.5), -1.2))), 0.0);
            }
            if(child is Branch) {
                child.addPoints(spline, root);
                branchNumber++;
            }
        }

        // going down on the right
        spline.add(root.globalToLocal(localToGlobal(new Point(et/2, -1))), tangentLength);
        if(isRoot()) {
            spline.add(root.globalToLocal(localToGlobal(new Point(st/2, 0))), 0.1);
            spline.add(root.globalToLocal(localToGlobal(new Point(st/2 * 1.5, 0.2))), 0.1);
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

        print("Drag start");
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
        if(!isDragging) return;
        isDragging = false;

        if(mode == "branch") {
            var mouse = new Vector(mouseX, mouseY);
            growChild(mouse.rads);
        }
    }

    void growChild(num absolute_angle) {
        Branch b = new Branch();
        b.rotation = absolute_angle - getAbsoluteAngle();
        b.thickness = thickness * 0.5;
        addChild(b);
    }

    Vector getTipPosition() {
        var p = view.globalToLocal(localToGlobal(new Point(0, 0)));
        return new Vector(p.x, p.y);
    }

    // DisplayObject hitTestInput(num localX, num localY) {
    //     return (shape.hitTestInput(localX, localY) != null) ? this : null;
    // }
}
