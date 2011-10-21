#include "Background.hpp"
#include "GameApp.hpp"
#include <string>

Background::Background(const std::string& type, float speed) {
    mType = "background";
    mMarkedForRemoval = false;
    mSpeed = speed; // has to be negative
    
    mScale = 1.f;

    mSprite1.SetTexture(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetTexture(type));

    mSprite2.SetTexture(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetTexture(type));
    mSprite2.FlipX( true ); // background isn't really tileable, so let's make it!

    mSprite3.SetTexture(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetTexture(type));
}

Background::~Background() {}

void Background::Update(float time_diff) {
    if(mPosition.x < -1600) {
        mPosition.x = 0;
    }
    mSprite1.SetPosition(mPosition.x, mPosition.y);

    mSprite2.SetPosition(mPosition.x + 800, mPosition.y);

    mSprite3.SetPosition(mPosition.x + 1600, mPosition.y);

    mPosition.x += mSpeed;

    UpdateAllAttachments(time_diff);
}

void Background::Draw(sf::RenderTarget* target) {
    target->Draw(mSprite1);
    target->Draw(mSprite2);
    target->Draw(mSprite3);

    DrawAllAttachments(target);
}
