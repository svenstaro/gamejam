#include "Ship.hpp"

Ship::Ship(const float scale, const std::string& image_path) {
	m_scale = scale;
	m_image_path = image_path;
}

Ship::~Ship() {}

void Ship::Spawn(const float pos_x, const float pos_y, b2World* world) {
	m_texture.LoadFromFile(m_image_path);
	m_texture.SetSmooth(false);
	m_sprite.SetTexture(m_texture);
	m_sprite.SetScale(m_scale, m_scale);
	m_sprite.SetPosition(pos_x, pos_y);

	sf::Vector2f size = m_sprite.GetSize();
	m_radius = (size.x + size.y) / 8;

	m_max_health = 1000;
	m_health = m_max_health;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos_x, pos_y);
	m_body = world->CreateBody(&bodyDef);

	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = m_radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicCircle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	m_body->CreateFixture(&fixtureDef);
}

void Ship::Update() {
    sf::Vector2i mouse_pos = sf::Mouse::GetPosition();
	MoveTo(mouse_pos.x, mouse_pos.y);

	b2Vec2 position = m_body->GetPosition();
	float32 angle = m_body->GetAngle();
	sf::Vector2f size = m_sprite.GetSize();
	m_sprite.SetPosition(position.x + size.x / 2, 
						 position.y - size.y / 2);
	m_sprite.SetRotation(angle - 180);
}

const sf::Sprite& Ship::GetDrawable() {
	return m_sprite;
}

const sf::Shape Ship::GetDebugDrawable() {
	b2Vec2 position = m_body->GetPosition();
	// This is probably very performance intensive.
	sf::Shape debug_shape = sf::Shape::Circle(position.x, position.y, m_radius, sf::Color(200, 10, 10, 100));
	return debug_shape;
}

const b2ContactEdge* Ship::GetContactList() {
	return m_body->GetContactList();
}

void Ship::SetHealth(const float health) {
	m_health = health;
}

float Ship::GetHealth() const {
	return m_health;
}

void Ship::SetMaxHealth(const float health) {
	m_max_health = health;
}

float Ship::GetMaxHealth() const {
	return m_max_health;
}

void Ship::MoveTo(const float pos_x, const float pos_y) {
	m_body->SetTransform(b2Vec2(pos_x, pos_y), -90);
}
