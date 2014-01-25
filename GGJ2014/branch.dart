part of game;

class Branch extends Sprite {
    num thickness = 0.1;
    num baseRotation = 0.0;

    Branch() {
        this.y = -1;

        this.onEnterFrame.listen(this._onEnterFrame);
    }

    void _onEnterFrame(EnterFrameEvent e) {
        this.rotation = this.baseRotation + (PI * .5 - this.baseRotation) * Wind.power * 0.01;

        num st = getStartThickness();
        num et = thickness;

        this.graphics.clear();
        // this.graphics.moveTo(-et/2, -1);
        // this.graphics.lineTo(-st/2, 0);
        // this.graphics.lineTo( st/2, 0);
        // this.graphics.lineTo( et/2, -1);
        // this.graphics.closePath();

        if(!(this.parent is Branch)) {
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

        for(int i = 0; i < numChildren; i++) {
            var child = getChildAt(i);
            if(i >= 1) {
                points.add(root.globalToLocal(localToGlobal(new Point(et*(i*1.0/numChildren - 0.5), -1.2))));
            }
            if(child is Branch) {
                child.addPoints(points, root);
            }
        }

        // going down on the right
        points.add(root.globalToLocal(localToGlobal(new Point( et/2, -1))));
        if(identical(root, this)) {
            points.add(root.globalToLocal(localToGlobal(new Point( st/2,  0))));
        }
    }

    num getStartThickness() {
        if(this.parent is Branch) {
            return this.parent.thickness;
        } else {
            return this.thickness;
        }
    }
}
