#include <cmath>

#include "Blob.hpp"
#include "GameApp.hpp"
#include "EntityAttachment.hpp"

Blob::Blob() {
    mType = "blob";
    mMarkedForRemoval = false;

    mSprite.SetTexture(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetTexture("photon"));
    mSprite.SetOrigin(mSprite.GetSize().x / 2, mSprite.GetSize().y / 2);
    mSprite.SetBlendMode(sf::Blend::Add);
    mSprite.SetColor(sf::Color(100,200,255));
    mScale = 0.7f;

    mPosition = Vector2D(800 / 2, 600 / 2);
    mSpeed = 80.f;

    mCurrentPathSegment = 0;
    mPathSegmentProgress = 0.f;

    SetPhotonColor(sf::Color(sf::Color(50,100,255)));

    boost::shared_ptr<ParticleSystem> partsys = boost::shared_ptr<ParticleSystem>(new ParticleSystem());
    partsys->SetPosition(Vector2D(0.f, 0.f));
    partsys->SetDirection(Vector2D(1.f, 1.f));
    ParticleEmitter* partemit = new ParticleEmitter(); 
    partemit->SetStartColor(sf::Color(200,200,255));
    partemit->SetEndColor(sf::Color(255,255,255));
    partemit->SetStartAlpha(50);
    partemit->SetEndAlpha(0);
    partemit->SetStartScale(1.5f);
    partemit->SetEndScale(3.f);
    partsys->AddEmitter(partemit);
    
    Attach(partsys, Vector2D(0, 0), 0.f, RestraintSettings());
}

Blob::~Blob() {}

void Blob::Update(float time_diff) {
    /*if(mState == FOLLOW_PATH) {
        float move_distance = time_diff * mSpeed;
        while(move_distance > 0 && mCurrentPathSegment < mPath.GetPointsCount() - 1) {
            Vector2D segment = mPath.GetSegmentVector(mCurrentPathSegment); 
            Vector2D point = mPath.GetPoint(mCurrentPathSegment);
            float segment_length = segment.Magnitude();
            float segment_length_gone = segment_length * mPathSegmentProgress;
            float segment_length_left = segment_length - segment_length_gone;

            if(move_distance > segment_length_left) {
                // continue on the next segment
                mCurrentPathSegment++;
                mPathSegmentProgress = 0.f;
                move_distance -= segment_length_left;
            } else {
                // move to point on this segment
                float new_segment_length_gone = segment_length_gone + move_distance;
                mPathSegmentProgress = new_segment_length_gone / segment_length;
                move_distance = 0;
            }
        }

        float total_progress = mCurrentPathSegment * 1.f / mPath.GetPointsCount();
        if(total_progress > 0.8) {
            float acceleration_progress = (total_progress - 0.8) * 5.f;
                mSpeed = 80.f - 50.f * acceleration_progress;
        }

        Vector2D segment = mPath.GetSegmentVector(mCurrentPathSegment); 
        Vector2D point = mPath.GetPoint(mCurrentPathSegment);
        mPosition = point + segment * mPathSegmentProgress;

        if(mCurrentPathSegment == mPath.GetPointsCount() - 1) {
            // finished trail
            mState = RETURN_TO_PLAYER;
        } else {
            mDirection = segment;
            mDirection.Normalize();
        }
    } **/
        
    if(mState ==  GOTO_TARGET || mState == GOTO_TARGET_THEN_RETURN) {
        Vector2D diff = mTarget - mPosition;
        float d = diff.Magnitude();
        if(d < 5) {
            if(mState == GOTO_TARGET_THEN_RETURN) {
                mState = RETURN_TO_PLAYER;
            }
        } else {
            diff.Normalize();
            mDirection = diff;
            mPosition += mDirection * mSpeed * time_diff;
        }

    } else if(mState == RETURN_TO_PLAYER) {
        Vector2D diff = Vector2D(400,300) - mPosition;
        if(diff.Magnitude() < 10) {
            // TODO: close to target... merge them

            if(GameApp::get_mutable_instance().GetWorld().GetEnergyOfEntity(this) > 0)
                mSpeed *= 0.01;

            Photon* p;
            while(p = GameApp::get_mutable_instance().GetWorld().GetPhotonWithTarget(this)) {
                Player* pl = GameApp::get_mutable_instance().GetWorld().GetPlayer();
                p->SetTarget(pl);
            }
            // stop particle emission
            EntityAttachment& a = mAttachments.front();
            ParticleSystem* sys = (ParticleSystem*)(a.GetEntity());

            if(sys->GetParticleCount() > 0) {
                sys->StopEmitting();
            } else {
            }
            
        }
        
        diff.Normalize();
        mDirection.Normalize();
        mDirection += diff * time_diff * 2;

        mDirection.Normalize();
        mPosition += mDirection * mSpeed * time_diff;

        EntityAttachment& a = mAttachments.front();
        ParticleSystem* sys = (ParticleSystem*)(a.GetEntity());
        float al = sys->GetParticleCount() / 30.f;
        if(al > 1) al = 1.f;
        mSprite.SetColor(sf::Color(mSprite.GetColor().r, mSprite.GetColor().g, mSprite.GetColor().b, al * 255));
        if(sys->GetParticleCount() == 0)
            MarkForRemoval();
    }

    mSprite.SetPosition(mPosition.x, mPosition.y);
    mSprite.SetRotation(- 90 - Vector2D::rad2Deg(mDirection.Rotation()));
    mSprite.SetScale(mScale, mScale);
    UpdateAllAttachments(time_diff);
}

void Blob::Draw(sf::RenderTarget* target) {
    target->Draw(mSprite);

    /*if(mState != RETURN_TO_PLAYER)
        mPath.Draw(target, mCurrentPathSegment);*/
    if(mState != RETURN_TO_PLAYER) {
        sf::Shape line = sf::Shape::Line(mPosition.x, mPosition.y, mTarget.x, mTarget.y, 1.f, sf::Color::White);
        target->Draw(line);
    }

    DrawAllAttachments(target);
}

/*
Path& Blob::GetPath() {
    return mPath;
}

void Blob::StartPath() {
    mPath.AddStartPoint(mPosition);
    mState = FOLLOW_PATH;
}
*/

sf::Color Blob::GetPhotonColor() {
    return mPhotonColor;
}

void Blob::SetPhotonColor(sf::Color color) {
    mPhotonColor = color;
}

void Blob::SetTarget(Vector2D pos) {
    mTarget = pos;
    mState = GOTO_TARGET;
}

void Blob::Release() {
    mState = GOTO_TARGET_THEN_RETURN; 
}

