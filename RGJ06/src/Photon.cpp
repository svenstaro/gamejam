#include "Photon.hpp"

#include "GameApp.hpp"

Photon::Photon(Vector2D pos) {
    mType = "photon";
    mMarkedForRemoval = false;

    mSprite.SetImage(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("photon"));
    mSprite.SetOrigin(mSprite.GetImage()->GetWidth() / 2.f, mSprite.GetImage()->GetHeight() / 2.f);
    mSprite.SetBlendMode(sf::Blend::Add);

    SetPosition(pos + Vector2D(sf::Randomizer::Random(-30.f,30.f), sf::Randomizer::Random(-30.f,30.f)));
    SetSpeed(sf::Randomizer::Random(40.f,80.f));
    SetDirection(Vector2D(sf::Randomizer::Random(-1.f,1.f), sf::Randomizer::Random(-1.f,1.f)));

    mTarget = NULL;
}

void Photon::SetTarget(Entity* entity) {
    mTarget = entity;
}

Entity* Photon::GetTarget() {
    return mTarget;
}

void Photon::Update(float time_diff) {
    Vector2D diff;
    if(mTarget != NULL)
        diff = mTarget->GetPosition() - mPosition;

    float fac = 1;
    if(TargetIsPlayer()) 
        fac = 0.1;
    SetDirection(mDirection + diff * 0.01 * fac);
    mScale = sin(GetLifeTime() * 10) * 0.02 + 0.2;
    if(GetLifeTime() < 1)
        mScale *= GetLifeTime();

    mDirection.Normalize();
    mPosition += mDirection * mSpeed * (TargetIsPlayer()?1:3) * time_diff;
}

void Photon::Draw(sf::RenderTarget* target) {
    mSprite.SetPosition(mPosition.x, mPosition.y);
    mSprite.SetScale(mScale, mScale);
    mSprite.SetColor(mTarget->GetPhotonColor());
    target->Draw(mSprite);
}

bool Photon::TargetIsPlayer() {
   return mTarget == GameApp::get_mutable_instance().GetWorld().GetPlayer(); 
}
    
void Photon::CheckOpponentCollision() {
    BOOST_FOREACH(Photon* p, GameApp::get_mutable_instance().GetWorld().GetEntitiesByType<Photon>()) {
        if( (GetTarget()->GetType() == "enemy") != (p->GetTarget()->GetType() == "enemy") ) {
            // is opponent
            if((p->GetPosition() - GetPosition()).Magnitude() < 10) {
                // collision
                World& w = GameApp::get_mutable_instance().GetWorld();
                int m_e = w.GetEnergyOfEntity(GetTarget());    // my energy
                int p_e = w.GetEnergyOfEntity(p->GetTarget()); // other's energy
                bool other_is_player = p->GetTarget()->GetType() == "player";
                if( m_e > p_e || other_is_player || (m_e == p_e && sf::Randomizer::Random(0,1) == 1) ) {
                    // I am stronger, delete the other one
                    p->MarkForRemoval();
                    return;
                } else {
                    // he is stronger, delete me
                    MarkForRemoval();
                    return;
                }
            }
        }
    }
}
