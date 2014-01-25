part of game;

class SplinePoint {
    Spline spline;
    Vector point;
    double length;
    SplinePoint(this.spline, this.point, this.length);

    int get index           => spline.points.indexOf(this);
    SplinePoint get prev         => index > 0 ? spline.points[index-1] : this;
    SplinePoint get next         => index < spline.points.length - 1 ? spline.points[index+1] : this; 
    Vector get tangent      => (next.point - prev.point).normalize();
    Vector get inHandle     => point - tangent.scale(length);
    Vector get outHandle    => point + tangent.scale(length);
}

class Spline {
    List<SplinePoint> points = new List<SplinePoint>();

    void add(var point, double length) {
        if(point is Point) point = new Vector(point.x, point.y);
        if(!(point is Vector)) throw new Exception("Argument 'point' must be of type [Vector] or [Point].");
        points.add(new SplinePoint(this, point, length));
    }

    void generatePath(Graphics g) {
        g.beginPath();
        g.moveTo(points[0].point.x, points[0].point.y);
        for(int i = 0; i < points.length - 1; ++i) {
            var h1 = points[i].outHandle;
            var h2 = points[i+1].inHandle;
            var p = points[i+1].point;

            g.bezierCurveTo(h1.x, h1.y, h2.x, h2.y, p.x, p.y);
        }
        // g.closePath();
    }
}
