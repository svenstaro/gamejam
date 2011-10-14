#include "AnimatedSprite.hpp"

#include <iostream>
#include <SFML/Graphics/Sprite.hpp>

using namespace sf;

// constructors
AnimatedSprite::AnimatedSprite() {
	SetSubRect(IntRect(0,0,1,1));
	mFrameNumber = 0;
	mFPS = 1;
	mNumFrames = 1;
	mTimeSinceLastFrame = 0;
}

AnimatedSprite::AnimatedSprite(const Texture& Texture, const Vector2f& Position, const Vector2f& Scale, float Rotation, const Color& Col) {
	SetTexture(Texture);
	SetPosition(Position);
	SetScale(Scale);
	SetRotation(Rotation);
	SetColor(Col);
	SetSubRect(IntRect(0,0,1,1));
	mFrameNumber = 0;
	mFPS = 1;
	mNumFrames = 1;
	mTimeSinceLastFrame = 0;
}


void AnimatedSprite::SetFPS(float fps) {
	mFPS = fps;
}

float AnimatedSprite::GetFPS() const {
	return mFPS;
}

void AnimatedSprite::SetNumFrames(sf::Uint16 num_frames) {
	mNumFrames = num_frames;
}

sf::Uint16 AnimatedSprite::GetNumFrames() const {
	return mNumFrames;
}

void AnimatedSprite::SetSubRectSize(sf::Vector2f sub_rect_size) {
	mSubRectSize = sub_rect_size;
}

const sf::Vector2f AnimatedSprite::GetSubRectSize() const {
	return mSubRectSize;
}

void AnimatedSprite::SetSubRectOffset(sf::Vector2f sub_rect_offset) {
	mSubRectOffset = sub_rect_offset;
}

const sf::Vector2f AnimatedSprite::GetSubRectOffset() const {
	return mSubRectOffset;
}

void AnimatedSprite::Update(const float time_delta) {
	// only update mFrameNumber, the graphics (subrect etc.) will be calculated during render
	mTimeSinceLastFrame += time_delta;
	while (mTimeSinceLastFrame > 1.f/mFPS) {
		mFrameNumber++;
		if (mFrameNumber >= mNumFrames)
			mFrameNumber = 0;
		mTimeSinceLastFrame -= 1.f/mFPS;
	}
}

void AnimatedSprite::Render(RenderTarget&, Renderer& renderer) const
{
	// Get the sprite size
	float width  = mSubRectSize.x;
	float height = mSubRectSize.y;

	// Check if the texture is valid, and calculate the texture coordinates
	FloatRect coords;
	IntRect subrect(mSubRectOffset.x + mSubRectSize.x * mFrameNumber, mSubRectOffset.y, mSubRectSize.x, mSubRectSize.y);
	if (GetTexture())
		coords = GetTexture()->GetTexCoords(subrect);

	// Compute the texture coordinates
	float left   = coords.Left;
	float top    = coords.Top;
	float right  = coords.Left + coords.Width;
	float bottom = coords.Top + coords.Height;

	// Bind the texture
	renderer.SetTexture(GetTexture());

	// Draw the sprite's geometry
	renderer.Begin(Renderer::TriangleStrip);
		renderer.AddVertex(0,     0,      left,  top);
		renderer.AddVertex(width, 0,      right, top);
		renderer.AddVertex(0,     height, left,  bottom);
		renderer.AddVertex(width, height, right, bottom);
	renderer.End();
}
