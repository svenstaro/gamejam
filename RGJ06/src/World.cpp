#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

#include <string>
#include <vector>

#include "World.hpp"
#include "Blob.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Vector2D.hpp"
#include "Photon.hpp"
#include "Background.hpp"

World::World() {}

World::~World() {}

void World::Initialize() {
    Reset();

    mDebug = true;
    mTimeSinceLastEnemy = 0;
    mTimeSinceLastPhoton = 0;
    mTimeSinceLastPhotonTransfered = 0;

    AddEntity(new Background("background", -1.0f));
    AddEntity(new Background("foreground", -3.0f));
    AddEntity(new Background("foreground", -5.0f));
    AddEntity(new Player());

    auto entities = GetEntitiesByType<Player>();
    mPlayer = entities[0];

    ParticleSystem* partsys = new ParticleSystem();
    partsys->SetName("mouse_partsys");
    partsys->SetPosition(Vector2D(800 / 2, 600 / 2));
    partsys->SetDirection(Vector2D(1, 1));
    ParticleEmitter* emitter = new ParticleEmitter(); 
    emitter->SetSpread(360.f);
    emitter->SetTimeToLive(0.2f);
    emitter->SetStartColor(sf::Color(50, 100, 200));
    emitter->SetEndColor(sf::Color(255, 255, 255));
    emitter->SetStartAlpha(255);
    emitter->SetEndAlpha(255);
    emitter->SetStartScale(0.3f);
    emitter->SetEndScale(0.05f);
    emitter->SetRate(100.f);
    emitter->SetBlendMode(sf::Blend::Add);
    emitter->SetImageName("photon");
    partsys->AddEmitter(emitter);
    AddEntity(partsys);
}

void World::Reset() {
    mCurrentBlob = NULL;

    mLevel = 1;
    mScore = 0;
}

void World::HandleEvent(const sf::Event& event) {
    auto mouse_partsys = GetEntityByName<ParticleSystem>("mouse_partsys");
    mouse_partsys->SetPosition(Vector2D(sf::Mouse::GetPosition().x,
                                        sf::Mouse::GetPosition().y));

    if(event.Type == sf::Event::KeyPressed) {
        if(event.Key.Code == sf::Keyboard::D) {
            mDebug = !mDebug;
        } else if(event.Key.Code == sf::Keyboard::Space) {
            AddPhoton(GetPlayer());
        }
    }

    if(event.Type == sf::Event::MouseButtonPressed) {
        Vector2D mp(event.MouseButton.X, event.MouseButton.Y);
        if(event.MouseButton.Button == sf::Mouse::Left) {
            if(PointWithinPlayerRange(mp)) {
                //std::cout << "Click on player" << std::endl;
                if(mCurrentBlob == NULL) {
                    // no blob selected, create one
                    AddEntity(new Blob());
                    Entity* e = &mEntities.back();
                    mCurrentBlob = (Blob*)e;
                    mCurrentBlob->SetPosition(mp);
                }
            } 
        }
    }
    if(event.Type == sf::Event::MouseButtonReleased) {
        Vector2D mp(event.MouseButton.X, event.MouseButton.Y);
        if(mCurrentBlob != NULL) {
            mCurrentBlob->Release();
            mCurrentBlob = NULL;
        }
    }

    /*if(event.Type == sf::Event::MouseMoved) {
        Vector2D mp(event.MouseMove.X, event.MouseMove.Y);
        if(!PointWithinPlayerRange(mp) && mCurrentBlob != NULL) {
            // TODO: add path point
            Path& p = mCurrentBlob->GetPath();
            float threshold = 5.f;
            if(p.GetPointsCount() == 0 || (p.GetLastPoint()-mp).Magnitude() > threshold ) {
                //p.AddPoint(mp);
            }
        }
    }*/
}

void World::Update(float time_diff) {

    mTimeSinceLastPhotonTransfered += time_diff;
    if(sf::Mouse::IsButtonPressed(sf::Mouse::Left)) {
        Vector2D mp(sf::Mouse::GetPosition().x,
                    sf::Mouse::GetPosition().y);
        int player_energy = GetEnergyOfEntity(GetPlayer());
        if(PointWithinPlayerRange(mp) && mCurrentBlob != NULL && player_energy > 0) {
            if(mTimeSinceLastPhotonTransfered > (3.0 / (player_energy + GetEnergyOfEntity(mCurrentBlob)) )) {
                // add one photon to the blob
                GetPhotonWithTarget(GetPlayer())->SetTarget(mCurrentBlob);
                mTimeSinceLastPhotonTransfered = 0;
            }
        }
        if(mCurrentBlob != NULL) {
            mCurrentBlob->SetTarget(mp);
        }
    }


    BOOST_FOREACH(Entity& entity, mEntities) {
        entity.Update(time_diff);
    }

    // We got some better collision handling now!
    //const Vector2D& player_pos = Vector2D(800/2, 600/2);
    //mEntities.erase_if(boost::bind(&Vector2D::Distance, boost::bind(&Entity::GetPosition, _1), player_pos) < 80 &&
    //                    boost::bind(&Entity::GetType, _1) == "enemy");

    mScore += time_diff;
    mLevel = mScore/15;
    
    mTimeSinceLastPhoton += time_diff;
    if(mTimeSinceLastPhoton > 5) {
        AddPhoton(GetPlayer());
        mTimeSinceLastPhoton = 0;
    }

    mTimeSinceLastEnemy += time_diff;
    if(mTimeSinceLastEnemy >= GetEnemyInterval()) {
        AddEntity(new Enemy());
        mTimeSinceLastEnemy = 0;
    }
    
    BOOST_FOREACH(Entity& e, mEntities) {
        if(e.GetType() == "photon") {
            Photon& p = (Photon&)e;
            if(p.GetTarget()->GetType() == "enemy") {
                p.CheckOpponentCollision();
            }
        }
    }
    mEntities.erase_if(boost::bind(&Entity::IsMarkedForRemoval, _1));
}

void World::Draw(sf::RenderTarget* target) {
    // THIS SOLUTION IS SHIT AND INEFFICIENT BUT IT WORKS
    
    // Draw particle systems first ...
    BOOST_FOREACH(Entity& entity, mEntities) {
        if(entity.GetType() == "background")
            entity.Draw(target);
    }

    // Draw particle systems first ...
    BOOST_FOREACH(Entity& entity, mEntities) {
        if(entity.GetType() == "particlesystem" && entity.GetType() != "background")
            entity.Draw(target);
    }

    // ... then draw everything else!
    BOOST_FOREACH(Entity& entity, mEntities) {
        if(entity.GetType() != "particlesystem" && entity.GetType() != "background")
            entity.Draw(target);
    }

    sf::Uint32 score = sf::Uint32(mScore);
    sf::Text score_text("Time: "+boost::lexical_cast<std::string>(score));
    score_text.SetCharacterSize(10);
    score_text.SetPosition(5,5);
    target->Draw(score_text);

    if(mDebug) {
        int energy = GetEnergyOfEntity(GetPlayer());
        sf::Text energy_text("Interval: " + boost::lexical_cast<std::string>(GetEnemyInterval()) + " - Energy: "+boost::lexical_cast<std::string>(energy));
        energy_text.SetCharacterSize(10);
        energy_text.SetPosition(5,18);
        target->Draw(energy_text);

        if(mCurrentBlob != NULL) {
            sf::Uint32 energy2 = sf::Uint32(GetEnergyOfEntity(mCurrentBlob));
            sf::Text energy2_text("Blob Energy: "+boost::lexical_cast<std::string>(energy2));
            energy2_text.SetCharacterSize(10);
            energy2_text.SetPosition(5,33);
            target->Draw(energy2_text);
        }
    }

    // = GUI
    
    if(mCurrentBlob != NULL) {
        int w = 100;
        int h = 10;
        int b = 2;
        
        int x = 800 / 2 - w / 2;
        int y = 600 / 2 - h / 2 - 60;

        sf::Shape outer = sf::Shape::Rectangle(x - 2 * b, y - 2 * b, w + 4 * b, h + 4 * b, sf::Color::White, b, sf::Color::White);
        outer.EnableFill(false);
        outer.EnableOutline(true);
        target->Draw(outer);

        float p = 1.0f * GetEnergyOfEntity(mCurrentBlob) / (GetEnergyOfEntity(mCurrentBlob) + GetEnergyOfEntity(GetPlayer()));
        sf::Shape inner = sf::Shape::Rectangle(x, y, w * p, h, sf::Color::White);
        inner.EnableFill(true);
        target->Draw(inner);

        sf::Text energy2_text(boost::lexical_cast<std::string>(GetEnergyOfEntity(mCurrentBlob)));
        energy2_text.SetCharacterSize(10);
        energy2_text.SetColor(sf::Color::White);
        energy2_text.SetPosition(x - round(energy2_text.GetRect().Width) - 10, y+h/2 - round(energy2_text.GetRect().Height / 2));
        target->Draw(energy2_text);
    }
}

void World::AddEntity(Entity* e) {
    e->Update(0);
    mEntities.push_back(e);
}

Player* World::GetPlayer() {
    return mPlayer;
}
    
Photon* World::GetPhotonWithTarget(Entity* target) {
    BOOST_FOREACH(Entity& e, mEntities) {
        if(e.GetType() == "photon") {
            Photon* p = (Photon*)&e;
            if(p->GetTarget() == target) {
                return p;
            }
        }
    }
    return NULL;
}
    
bool World::PointWithinPlayerRange(Vector2D point, int radius) {
    float d = (GetPlayer()->GetPosition() - point).Magnitude();
    float r = GetPlayer()->GetRadius() + radius;
    return d <= r;
}

void World::AddPhoton(Entity* target) {
    AddEntity(new Photon(target->GetPosition()));
    Photon* p = (Photon*)&mEntities.back();
    p->SetTarget(target);
}
    
float World::GetEnemyInterval() {
    if(mLevel == 0) mLevel = 1;
    float eps = 0.1 * mLevel;
    return 1.f / eps;
}
    
int World::GetLevel() {
    return mLevel;
}

int World::GetEnergyOfEntity(Entity* e) {
    int c = 0;
    BOOST_FOREACH(Photon* p, GetEntitiesByType<Photon>()) {
        if(p->GetTarget() == e) {
            c++;
        }
    }
    return c;
}
