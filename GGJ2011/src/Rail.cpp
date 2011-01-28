#include "Rail.hpp"

Rail::Rail() {
	mLastPointSet = 0;
}

void Rail::Draw(sf::RenderTarget* target, sf::Color bordercolor) {
	if(IsFinished()) {
		// debug: line
		sf::Shape shape = sf::Shape::Line(mPoint1.x, mPoint1.y, mPoint2.x, mPoint2.y, 10.f, sf::Color(255,128,0), 5.f, bordercolor);
		target->Draw(shape);
	} else {
		// draw p1
		sf::Shape shape = sf::Shape::Circle(mPoint1.x, mPoint1.y, 10.f, sf::Color(255,128,0), 5.f, bordercolor);
		target->Draw(shape);
	}
}

void Rail::SetNextPoint(Vector2D point) {
	if(mLastPointSet == 0) {
		mPoint1 = point;
	} else if(mLastPointSet == 1) {
		mPoint2 = point;
	}
	++mLastPointSet;
}

void Rail::Save(boost::property_tree::ptree* pt, int id) {
	std::string sid = boost::lexical_cast<std::string>(id);
	pt->add("rails."+sid+".1.x", mPoint1.x);
	pt->add("rails."+sid+".1.y", mPoint1.y);
	pt->add("rails."+sid+".2.x", mPoint2.x);
	pt->add("rails."+sid+".2.y", mPoint2.y);
}

void Rail::Load(boost::property_tree::ptree* pt, int id) {
	std::string sid = boost::lexical_cast<std::string>(id);
	mPoint1.x = pt->get<float>("rails."+sid+".1.x");
	mPoint1.y = pt->get<float>("rails."+sid+".1.y");
	mPoint2.x = pt->get<float>("rails."+sid+".2.x");
	mPoint2.y = pt->get<float>("rails."+sid+".2.y");
	mLastPointSet = 2;
}

Vector2D Rail::GetCenter() {
	return (mPoint1 + mPoint2) / 2;
}

bool Rail::IsFinished() const {
	return mLastPointSet >= 2;
}
