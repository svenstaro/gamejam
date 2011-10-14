#include "TiledSprite.hpp"

#include <iostream>
#include <SFML/Graphics/Sprite.hpp>

using namespace sf;

// constructors
TiledSprite::TiledSprite() {
	SetSubRect(IntRect(0,0,1,1));
}

TiledSprite::TiledSprite(const Texture& Img, const Vector2f& Position, const Vector2f& Scale, const float Rotation, const Color& Col) {
	SetTexture(Img);
	SetPosition(Position);
	SetScale(Scale);
	SetRotation(Rotation);
	SetColor(Col);
	SetSubRect(IntRect(0,0,1,1));
}

void TiledSprite::Render(RenderTarget&, Renderer& renderer) const {
	const Texture* texture = GetTexture();


    const IntRect& sub_rect = GetSubRect();

	// Get the sprite size


    float Width  = static_cast<float>(sub_rect.Width);
	float Height = static_cast<float>(sub_rect.Height);
	float subw = Width / 3;  // 8
	float subh = Height / 3; // 8
	float sw = GetScale().x; // 200
	float sh = GetScale().y; // 30

    // Check if the texture is valid
	if(texture) {
		// Bind the texture
		renderer.SetTexture(texture);

		// 9 subrects
		for(int x = 0; x < 3; x++) {
			for(int y = 0; y < 3; y++) {
                // Calculate the texture coordinates
				FloatRect tc = texture->GetTexCoords(IntRect(x * subw, y * subh, subw, subh));

				float left = 0, top = 0, right = 0, bottom = 0;
				float hp = subw/sw;
				float vp = subh/sh;

					 if(x == 0)	{ left = 0;		right = hp;		}
				else if(x == 1)	{ left = hp;	right = 1-hp;	}
				else if(x == 2)	{ left = 1-hp;	right = 1;		}

					 if(y == 0)	{ top = 0;		bottom = vp;	}
				else if(y == 1)	{ top = vp;		bottom = 1-vp;	}
				else if(y == 2)	{ top = 1-vp;	bottom = 1;		}

				// Draw the sprite's geometry
				renderer.Begin(Renderer::TriangleStrip);
				renderer.AddVertex(left,  top,		tc.Left,			tc.Top);
				renderer.AddVertex(right, top,		tc.Left + tc.Width,	tc.Top);
				renderer.AddVertex(left,  bottom,	tc.Left,			tc.Top + tc.Height);
				renderer.AddVertex(right, bottom,	tc.Left + tc.Width,	tc.Top + tc.Height);
				renderer.End();
            }
		}
	}
}
