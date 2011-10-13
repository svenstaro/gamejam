#include "Coordinates.hpp"

#include <iostream>
#include "GameApp.hpp"
#include "Definitions.hpp"

// SETTER

void Coordinates::SetWorldPixel(const Vector2D coords) {
	mWorldPixel = coords;
}
void Coordinates::SetWorldFloat(const Vector2D coords) {
	mWorldPixel = coords * PIXELS_PER_METER;
}
void Coordinates::SetViewPixel(const Vector2D coords) {
	sf::View& view = GameApp::get_mutable_instance().GetView();
	mWorldPixel = coords + Vector2D(view.GetCenter().x - view.GetSize().x / 2.f, view.GetCenter().y - view.GetSize().y / 2.f); // coords + View Offset
}
void Coordinates::SetScreenPixel(const Vector2D coords) {
	sf::View& view = GameApp::get_mutable_instance().GetView();
	SetViewPixel(coords - Vector2D(view.GetViewport().Left, view.GetViewport().Top)); // coords - ViewPORT Offset
}
void Coordinates::SetScreenFloat(const Vector2D coords) {
	SetScreenPixel(Vector2D(coords.x * GameApp::get_mutable_instance().GetWindowSize().x, coords.y * GameApp::get_mutable_instance().GetWindowSize().y));
}

// GETTER

const Vector2D Coordinates::GetWorldPixel() const {
	return mWorldPixel;
}
const Vector2D Coordinates::GetWorldFloat() const {
	return mWorldPixel / PIXELS_PER_METER;
}
const Vector2D Coordinates::GetViewPixel() const {
	sf::View& view = GameApp::get_mutable_instance().GetView();
	return mWorldPixel - Vector2D(view.GetCenter().x - view.GetSize().x / 2.f, view.GetCenter().y - view.GetSize().y / 2.f); // mWorldPixel - View Offset
}
const Vector2D Coordinates::GetScreenPixel() const {
	sf::View& view = GameApp::get_mutable_instance().GetView();
	return GetViewPixel() + Vector2D(view.GetViewport().Left, view.GetViewport().Top); // ViewPixel + ViewPORT Offset
}
const Vector2D Coordinates::GetScreenFloat() const {
	return Vector2D(GetScreenPixel().x / GameApp::get_mutable_instance().GetWindowSize().x, GetScreenPixel().y / GameApp::get_mutable_instance().GetWindowSize().y);
}


// STATIC CONVERSTION FUNCTIONS
const Vector2D Coordinates::WorldFloatToWorldPixel(const Vector2D coords) {
	Coordinates tmp;
	tmp.SetWorldFloat(coords);
	return tmp.GetWorldPixel();
}

const Vector2D Coordinates::ScreenPixelToWorldFloat(const Vector2D coords) {
	Coordinates tmp;
	tmp.SetScreenPixel(coords);
	return tmp.GetWorldFloat();
}
const Vector2D Coordinates::WorldFloatToViewPixel(const Vector2D coords) {
	Coordinates tmp;
	tmp.SetWorldFloat(coords);
	return tmp.GetViewPixel();
}

const float Coordinates::WorldFloatToWorldPixel(const float length) {
    Coordinates tmp;
    tmp.SetWorldFloat(Vector2D(length, 0.f));
    return tmp.GetWorldPixel().x;
}

const Vector2D Coordinates::GetParallaxDisplacedPosition(const Vector2D position, float displacement_factor) {
	sf::View& view = GameApp::get_mutable_instance().GetView();
	Coordinates tmp;
	tmp.SetWorldPixel(Vector2D(view.GetCenter().x, view.GetCenter().y));
	Vector2D viewcenter_worldfloat = tmp.GetWorldFloat();
	Vector2D diff = position - viewcenter_worldfloat;
	return viewcenter_worldfloat + diff * displacement_factor;
}
