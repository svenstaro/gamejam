#include "CollisionPolygon.hpp"

#include "GameApp.hpp"

CollisionPolygon::CollisionPolygon() {}


void CollisionPolygon::AddPoint(Vector2D p) {
	mPoints.push_back(p);
}

sf::Shape CollisionPolygon::GetShape(sf::Color bordercolor) {
	sf::Shape s;
	s.SetOutlineThickness(1);
	BOOST_FOREACH(Vector2D& v, mPoints) {
		Coordinates tmp;
		tmp.SetWorldFloat(v);
		sf::Color c = sf::Color(0,0,0,0);
		if(GameApp::get_mutable_instance().GetAppMode() == AM_EDITOR )
			c.a = 255;
		s.AddPoint( tmp.GetWorldPixel().x, tmp.GetWorldPixel().y, sf::Color::Black, bordercolor);
	}
	return s;
}

void CollisionPolygon::Draw(sf::RenderTarget* target, sf::Color bordercolor) {
	sf::Shape s = GetShape(bordercolor);
	target->Draw(s);
	if (bordercolor.a > 0) {
		// draw center
		Coordinates c;
		c.SetWorldFloat(GetCenter());
		sf::Shape center = sf::Shape::Circle(c.GetWorldPixel().x, c.GetWorldPixel().y, 3, bordercolor, 1.f, sf::Color::White);
		target->Draw(center);

		// draw points
		BOOST_FOREACH(Vector2D& v, mPoints) {
			c.SetWorldFloat(v);
			sf::Shape point = sf::Shape::Rectangle(c.GetWorldPixel().x-2, c.GetWorldPixel().y-2, 5, 5, bordercolor, 1.f, sf::Color::White);
			target->Draw(point);
		}
	}
}

int CollisionPolygon::GetNumPoints() {
	return mPoints.size();
}

std::vector<Vector2D> CollisionPolygon::GetPoints() {
	return mPoints;
}

Vector2D CollisionPolygon::GetCenter() {
	float x = 0;
	float y = 0;
	int n = 0;
	BOOST_FOREACH(Vector2D& v, mPoints) {
		x+=v.x;
		y+=v.y;
		n++;
	}
	if (n == 0) return Vector2D(0,0);
	return Vector2D(x/n,y/n);
}

void CollisionPolygon::Save(boost::property_tree::ptree* pt, int id) {
	int i = 0;
	BOOST_FOREACH(Vector2D& v, mPoints) {
		pt->add("polygons."+boost::lexical_cast<std::string>(id)+"."+boost::lexical_cast<std::string>(i)+".x", v.x);
		pt->add("polygons."+boost::lexical_cast<std::string>(id)+"."+boost::lexical_cast<std::string>(i)+".y", v.y);
		++i;
	}
}

void CollisionPolygon::Load(boost::property_tree::ptree* pt, int id) {
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt->get_child("polygons."+boost::lexical_cast<std::string>(id))) {
		float x = pt->get<float>("polygons."+boost::lexical_cast<std::string>(id)+"."+boost::lexical_cast<std::string>(v.first.data())+".x");
		float y = pt->get<float>("polygons."+boost::lexical_cast<std::string>(id)+"."+boost::lexical_cast<std::string>(v.first.data())+".y");
		AddPoint(Vector2D(x,y));
	}	
}
