#ifndef BLOB_HPP
#define BLOB_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Path.hpp"

class Blob : public Entity {
public:
    enum BlobState {
        IDLE = 0,
        GOTO_TARGET = 1,
        GOTO_TARGET_THEN_RETURN = 2,
        RETURN_TO_PLAYER = 3
    };

    Blob();
    ~Blob();

    void Update(float time_diff);
    void Draw(sf::RenderTarget* target);

    //Path& GetPath();
    //void StartPath();
    void SetTarget(Vector2D pos);
    void Release();

    sf::Color GetPhotonColor();
    void SetPhotonColor(sf::Color color);
private:
    sf::Sprite mSprite;
    //Path mPath;
    Vector2D mTarget;
    BlobState mState;
    sf::Color mPhotonColor;

    int mCurrentPathSegment;
    float mPathSegmentProgress;
};

#endif
