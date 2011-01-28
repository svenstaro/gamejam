#ifndef COLLISIONPOLYGON_HPP
#define COLLISIONPOLYGON_HPP

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include "Vector2D.hpp"
#include "SFML/Graphics.hpp"
#include "Coordinates.hpp"

class CollisionPolygon {
public:
	CollisionPolygon();

	void AddPoint(Vector2D p);

	sf::Shape GetShape(sf::Color bordercolor = sf::Color::Black);
	void Draw(sf::RenderTarget* target, sf::Color bordercolor = sf::Color::Black);
	int GetNumPoints();

	std::vector<Vector2D> GetPoints();

	Vector2D GetCenter();
	void Save(boost::property_tree::ptree* pt, int id);
	void Load(boost::property_tree::ptree* pt, int id);
private:
	std::vector<Vector2D> mPoints;
};

#endif // COLLISIONPOLYGON_HPP
