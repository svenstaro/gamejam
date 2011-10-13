#include "DebugDraw.hpp"
#include "Definitions.hpp"

DebugDraw::DebugDraw(boost::shared_ptr<sf::RenderWindow> render_window)
{
	window = render_window;
}

const sf::Color DebugDraw::BtToSfColor(const btVector3& color) const {
	sf::Color result(sf::Uint8((color.x() * 255)), 
					 sf::Uint8((color.y() * 255)), 
					 sf::Uint8((color.z() * 255)));
	return result;
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
	sf::Shape line = sf::Shape::Line(from.x() * PIXELS_PER_METER,
									 from.y() * PIXELS_PER_METER,
									 to.x() * PIXELS_PER_METER,
									 to.y() * PIXELS_PER_METER,
									 1, BtToSfColor(color));
	window->Draw(line);
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& from_color, const btVector3& to_color) {
	sf::Shape line = sf::Shape::Line(from.x() * PIXELS_PER_METER,
									 from.y() * PIXELS_PER_METER,
									 to.x() * PIXELS_PER_METER,
									 to.y() * PIXELS_PER_METER,
									 1, BtToSfColor(from_color));
	line.SetPointColor(1, BtToSfColor(to_color));
	window->Draw(line);
}

void DebugDraw::drawSphere(btScalar radius, const btTransform& transform, const btVector3& color) {
	sf::Shape sphere = sf::Shape::Circle(transform.getOrigin().x() * PIXELS_PER_METER, transform.getOrigin().y() * PIXELS_PER_METER, radius * PIXELS_PER_METER, BtToSfColor(color), 1.f);
	sphere.EnableFill(false);
	sphere.EnableOutline(true);
	window->Draw(sphere);
}

void DebugDraw::drawSphere(const btVector3& p, btScalar radius, const btVector3& color) {
	sf::Shape sphere = sf::Shape::Circle(p.x(), p.y(), radius, BtToSfColor(color));
	window->Draw(sphere);
}
/*

   void DebugDraw::drawTriangle(const btVector3& v0, const btVector3& v1, const btVector3& v2, const btVector3&, const btVector3 &, const btVector3 &, const btVector3 &color, btScalar alpha) {
   }

   void DebugDraw::drawTriangle(const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar);

   btVector3 to = point_on_b + normal_on_b * distance;
   const btVector3& from = point_on_b;

   drawLine(from, to, color);
   }
   */

void DebugDraw::drawContactPoint(const btVector3& point_on_b, const btVector3& normal_on_b, btScalar distance, int life_time, const btVector3& color) {
	drawLine(point_on_b, point_on_b + normal_on_b * distance, color);
}

void DebugDraw::reportErrorWarning(const char* warning_string) {
	std::cout << warning_string << std::endl;
}

void DebugDraw::draw3dText(const btVector3& location, const char* text_string) {
	sf::String string(text_string);
	sf::Text text(string);
	text.SetPosition(location.x() * PIXELS_PER_METER,
			location.y() * PIXELS_PER_METER);
	window->Draw(text);
}

void DebugDraw::setDebugMode(int debugMode) {
	mDebugMode = debugMode;
}

int DebugDraw::getDebugMode() const {
	return mDebugMode;
}

/*
   void DebugDraw::drawAabb(const btVector3 &from, const btVector3 &to, const btVector3 &color);

   void DebugDraw::drawTransform(const btTransform &transform, btScalar orthoLen);

   void DebugDraw::drawArc(const btVector3 &center, const btVector3 &normal, const btVector3 &axis, btScalar radiusA, btScalar radiusB, btScalar minAngle, btScalar maxAngle, const btVector3 &color, bool drawSect, btScalar stepDegrees=btScalar(10.f));

   void DebugDraw::drawSpherePatch(const btVector3 &center, const btVector3 &up, const btVector3 &axis, btScalar radius, btScalar minTh, btScalar maxTh, btScalar minPs, btScalar maxPs, const btVector3 &color, btScalar stepDegrees=btScalar(10.f));
   */

   /*void DebugDraw::drawBox(const btVector3 &bbMin, const btVector3 &bbMax, const btVector3 &color) {
	   sf::Shape box = sf::Shape::Rectangle((bbMin * PIXELS_PER_METER).x(),
			   (bbMin * PIXELS_PER_METER).y(),
			   ((bbMax - bbMin) * PIXELS_PER_METER).x(),
			   ((bbMax - bbMin) * PIXELS_PER_METER).y(),
			   BtToSfColor(color), 1, BtToSfColor(color));
	   box.EnableFill(false);
	   box.EnableOutline(true);
	   window->Draw(box);	
   }

   void DebugDraw::drawBox(const btVector3 &bbMin, const btVector3 &bbMax, const btTransform &trans, const btVector3 &color) {
	   sf::Shape box = sf::Shape::Rectangle(((trans * bbMin) * PIXELS_PER_METER).x(),
			   ((trans * bbMin) * PIXELS_PER_METER).y(),
			   ((trans * bbMax - trans * bbMin) * PIXELS_PER_METER).x(),
			   ((trans * bbMax - trans * bbMin) * PIXELS_PER_METER).y(),
			   BtToSfColor(color), 1, BtToSfColor(color));
	   box.EnableFill(false);
	   box.EnableOutline(true);
	   window->Draw(box);	
   }
   */
