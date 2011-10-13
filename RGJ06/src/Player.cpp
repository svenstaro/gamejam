#include "Player.hpp"
#include "GameApp.hpp"

Player::Player() {
    mType = "player";
    mMarkedForRemoval = false;
    mPosition = Vector2D(800 / 2, 600 / 2);
    
    boost::shared_ptr<ParticleSystem> partsys = boost::shared_ptr<ParticleSystem>(new ParticleSystem());
    partsys->SetPosition(Vector2D(800 / 2, 600 / 2));
    partsys->SetDirection(Vector2D(1, 1));
    ParticleEmitter* emitter = new ParticleEmitter(); 
    emitter->SetSpread(360.f);
    emitter->SetStartColor(sf::Color(255, 200, 0));
    emitter->SetEndColor(sf::Color(255, 255, 255));
    emitter->SetStartAlpha(110);
    emitter->SetEndAlpha(0);
    emitter->SetStartScale(2.f);
    emitter->SetEndScale(10.f);
    emitter->SetRate(2.f);
    partsys->AddEmitter(emitter);
    
    Attach(partsys, Vector2D(0,0), 0.f, RestraintSettings());
}

Player::~Player() {}

void Player::Update(float time_diff) {
    UpdateAllAttachments(time_diff);
}

void Player::Draw(sf::RenderTarget* target) {
    DrawAllAttachments(target);
}

float Player::GetRadius() {
    return 50.f;
}

sf::Color Player::GetPhotonColor() {
    return sf::Color(255,200,0);
}
