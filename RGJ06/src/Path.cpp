#include "Path.hpp"

Path::Path() {}

Path::Path(Vector2D start) {
    AddPoint(start);
}

void Path::AddStartPoint(Vector2D point) {
    mPoints.push_front(point);
}

void Path::AddPoint(Vector2D point) {
    mPoints.push_back(point);
}

Vector2D Path::GetLastPoint() {
    return mPoints.back();
}

int Path::GetPointsCount() {
    return mPoints.size();
}

void Path::Draw(sf::RenderTarget* target, int start_point) {
    // don't draw if we don't have at least 2 points
    if(mPoints.size() <= 1) 
        return;

    int i = start_point;
    while(i < mPoints.size() - 1) {
        Vector2D p = mPoints[i];
        Vector2D u = GetPointNormal(i);
        Vector2D v = GetPointNormal(i + 1);
        Vector2D s = GetSegmentVector(i);

        u *= 1;
        v *= 1;

        sf::Shape poly;
        poly.AddPoint((p + u).GetSfmlVector(), sf::Color::White, sf::Color::White);
        poly.AddPoint((p - u).GetSfmlVector(), sf::Color::White, sf::Color::White);
        poly.AddPoint((p + s - v).GetSfmlVector(), sf::Color::White, sf::Color::White);
        poly.AddPoint((p + s + v).GetSfmlVector(), sf::Color::White, sf::Color::White);

        poly.EnableOutline(false);
        poly.EnableFill(true);

        target->Draw(poly);

        //target->Draw(sf::Shape::Circle(p.x, p.y, 1, sf::Color::Red));
        //std::cout << p << std::endl;
        /*Vector2D p1 = p+u;
        Vector2D p2 = p+v+s;
        Vector2D p3 = p-v+s;
        Vector2D p4 = p-u;
        target->Draw(sf::Shape::Line(p1.x, p1.y, p2.x, p2.y, 1, sf::Color::Red));
        target->Draw(sf::Shape::Line(p2.x, p2.y, p3.x, p3.y, 1, sf::Color::Red));
        target->Draw(sf::Shape::Line(p3.x, p3.y, p4.x, p4.y, 1, sf::Color::Red));
        target->Draw(sf::Shape::Line(p4.x, p4.y, p1.x, p1.y, 1, sf::Color::Red));*/

        ++i;
    }
}

Vector2D Path::GetSegmentVector(int segment) {
    if(segment >= mPoints.size() - 1)
        return Vector2D(0,0);
    return mPoints[segment+1] - mPoints[segment] ;
}

Vector2D Path::GetPointNormal(int point) {

    Vector2D p = mPoints[point];

    Vector2D s1 = GetSegmentVector(point - 1);
    Vector2D s2 = GetSegmentVector(point);

    if(s1.x == 0 && s1.y == 0) s1 = s2;
    if(s2.x == 0 && s2.y == 0) s2 = s1;

    if(s1.x == 0 && s1.y == 0) {
        return Vector2D(0,0);
    }

    float angle = Vector2D::Angle(s1, s2);
    s1.Normalize();
    s2.Normalize();

    Vector2D u = s1;
    u.Rotate(PI / 2 - angle / 2);
    u.Normalize();
    return u;
}


Vector2D Path::GetPoint(int point) {
    return mPoints[point];
}
