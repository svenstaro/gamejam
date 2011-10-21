#include "Enemy.hpp"
#include "GameApp.hpp"

#include "EntityAttachment.hpp"

Enemy::Enemy() {
    mType = "enemy";
    mMarkedForRemoval = false;
    
    mScale = 1.f;

    mSprite.SetTexture(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetTexture("photon"));
    mSprite.SetOrigin(mSprite.GetSize().x / 2, mSprite.GetSize().y / 2);
    mSprite.SetBlendMode(sf::Blend::Add);
    mSprite.SetColor(sf::Color(255,0,0));
    mScale = 0.7f;

    int rand_x = GameApp::Random(0, 800);
    int rand_y = GameApp::Random(0, 600);
    int border = GameApp::Random(0,3);
    if(border == 0)
        rand_x = 0;
    else if(border == 1)
        rand_x = 800;
    else if(border == 2)
        rand_y = 0;
    else if(border == 3)
        rand_y = 600;
    mPosition = Vector2D(rand_x, rand_y);

    mDirection = Vector2D(400, 300) - mPosition;
    mDirection.Normalize();

    mSpeed = 40.f;
            
    GeneratePhotons();

    boost::shared_ptr<ParticleSystem> partsys = boost::shared_ptr<ParticleSystem>(new ParticleSystem());
    partsys->SetPosition(Vector2D(800 / 2, 600 / 2));
    partsys->SetDirection(Vector2D(1, 1));
    ParticleEmitter* emitter = new ParticleEmitter(); 
    emitter->SetSpread(360.f);
    emitter->SetStartColor(sf::Color(255, 0, 0));
    emitter->SetEndColor(sf::Color(255, 255, 255));
    emitter->SetStartAlpha(100);
    emitter->SetEndAlpha(0);
    emitter->SetStartScale(1.5f);
    emitter->SetEndScale(3.f);
    emitter->SetRate(10.f);
    emitter->SetBlendMode(sf::Blend::Add);
    partsys->AddEmitter(emitter);
    
    Attach(partsys, Vector2D(0,0), 0.f, RestraintSettings());
}

void Enemy::GeneratePhotons() {
    int lvl = GameApp::get_mutable_instance().GetWorld().GetLevel();
    int e = GameApp::Random(1,lvl);

    for(int i = 0; i < e; ++i) {
        GameApp::get_mutable_instance().GetWorld().AddPhoton(this);
    }
}

Enemy::~Enemy() {}

void Enemy::Update(float time_diff) {
    float distance = (Vector2D(400,300) - mPosition).Magnitude();
    if (distance < 50) {
        mSpeed = 40.f * (distance / 50);
    } 
    if (distance < 1) {
        mSpeed = 0;
    }
    mPosition += mDirection * time_diff * mSpeed; // move enemy towards player

    mSprite.SetPosition(mPosition.x, mPosition.y);
    mSprite.SetRotation(mRotation);
    mSprite.SetScale(mScale, mScale);

    UpdateAllAttachments(time_diff);

    if(GameApp::get_mutable_instance().GetWorld().GetEnergyOfEntity(this) < 1) {
        EntityAttachment& a = mAttachments.front();
        ParticleSystem* sys = (ParticleSystem*) (a.GetEntity());
        mSprite.SetColor(sf::Color(mSprite.GetColor().r, mSprite.GetColor().g, mSprite.GetColor().b, (sys->GetParticleCount() / 30.f) * 255));
        if(sys->GetParticleCount() > 0) {
            sys->StopEmitting();
        } else {
            MarkForRemoval();
        }
    }
}

void Enemy::Draw(sf::RenderTarget* target) {
    target->Draw(mSprite);

    DrawAllAttachments(target);
}

sf::Color Enemy::GetPhotonColor() {
    return sf::Color(120,0,0);
}
