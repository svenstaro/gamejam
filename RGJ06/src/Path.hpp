#ifndef PATH_HPP
#define PATH_HPP

#include <deque>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Vector2D.hpp"

class Path {
public:
    Path();
    Path(Vector2D start);

    void AddStartPoint(Vector2D point);
    void AddPoint(Vector2D point);
    Vector2D GetLastPoint();
    int GetPointsCount();

    void Draw(sf::RenderTarget* target, int start_point);
    Vector2D GetSegmentVector(int segment);
    Vector2D GetPointNormal(int point);
    Vector2D GetPoint(int point);
private:
    std::deque<Vector2D> mPoints;
};

#endif
