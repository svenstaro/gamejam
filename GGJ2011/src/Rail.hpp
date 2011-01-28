#ifndef RAIL_HPP
#define RAIL_HPP

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include "Vector2D.hpp"
#include "SFML/Graphics.hpp"
#include "Coordinates.hpp"

#include "TiledSprite.hpp"

class Rail {
public:
	Rail();

	void Draw(sf::RenderTarget* target, sf::Color bordercolor = sf::Color::Black);

	void SetNextPoint(Vector2D point);

	void Save(boost::property_tree::ptree* pt, int id);
	void Load(boost::property_tree::ptree* pt, int id);

	Vector2D GetCenter();

	bool IsFinished() const;

	float ClosestPositionOnLine(Vector2D pixel_pos);
	Vector2D GetPointFromFloat(float f) const;

private:
	Vector2D mPoint1, mPoint2;
	float mStartPosition;
	int mLastPointSet;
	TiledSprite mTiledSprite;
};

#endif // RAIL_HPP

