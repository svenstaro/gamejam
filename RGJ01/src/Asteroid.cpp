#include "Asteroid.hpp"

#include "GameApp.hpp"

Asteroid::Asteroid(const float scale, const std::string& image_path) {
	m_scale = scale;
	m_image_path = image_path;
}

Asteroid::~Asteroid() {}

void Asteroid::Spawn(const float pos_x, const float pos_y, const float speed, b2World& world) {
	m_texture.LoadFromFile(m_image_path);
	m_texture.SetSmooth(false);
	m_sprite.SetTexture(m_texture);
	m_sprite.SetScale(m_scale, m_scale);
	m_sprite.SetPosition(pos_x, pos_y);

	sf::Vector2f size = m_sprite.GetSize();
	m_radius = (size.x + size.y) / 8;

	m_speed = speed;
	m_rotation = GameApp::Random(-10.f, 10.f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set(pos_x, pos_y);
	m_body = world.CreateBody(&bodyDef);

	b2CircleShape kinematicCircle;
	kinematicCircle.m_radius = m_radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &kinematicCircle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	m_body->CreateFixture(&fixtureDef);
	
	float random_speed_v = GameApp::Random(-100.f, 100.f);
	m_body->SetLinearVelocity(b2Vec2(-1.f * m_speed, random_speed_v));
	//m_body->SetAngularVelocity(m_rotation);
	
	m_sprite.SetOrigin(size.x / 2, size.y / 2);
}

void Asteroid::Update(const float elapsed) {
	b2Vec2 position = m_body->GetPosition();
	//float32 angle = m_body->GetAngle();
	//sf::Vector2f size = m_sprite.GetSize();
	m_sprite.SetPosition(position.x, position.y);
	//m_sprite.SetRotation(angle);
	m_sprite.Rotate(m_rotation * 0.1f);
}

const sf::Sprite& Asteroid::GetDrawable() const {
	return m_sprite;
}

const sf::Shape Asteroid::GetDebugDrawable() const {
	b2Vec2 position = m_body->GetPosition();
	// This is probably very performance intensive.
	sf::Shape debug_shape = sf::Shape::Circle(position.x, position.y, m_radius, sf::Color(200, 10, 10, 100));
	return debug_shape;
}

b2Body* Asteroid::GetBody() {
	return m_body;
}
