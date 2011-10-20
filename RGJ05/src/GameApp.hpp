#ifndef GAMEAPP_HPP
#define GAMEAPP_HPP

#include <iostream>

#include <SFML/Graphics.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/lexical_cast.hpp>

#include "Definitions.hpp"
#include "Vector2D.hpp"
#include "ResourceManager.hpp"
#include "Entity.hpp"
#include "World.hpp"

class GameApp : public boost::serialization::singleton<GameApp> {
public:
	GameApp();
	~GameApp();

	void Init();
	void Run();
	void Quit();

	void LoadWorld();

	void ToggleEditorMode();
	bool IsEditorMode();
    const Vector2D GetWindowSize() const;
	sf::View& GetView();
	void SetGuiPaintingMode(bool guipaint);
	Vector2D GetMousePosition() const;

	int GetNextId();
	void SetNextId(int id);
	sf::Color GetSpriteColor(const float alpha) const;

	void SetSubtext(const std::string& subtext);

	void SetWorldHearts(int h);
	int GetWorldHearts() const;

	World* GetWorldPtr();
	ResourceManager* GetResourceManagerPtr();
	boost::shared_ptr<sf::RenderWindow> GetRenderWindowPtr();
private:
	boost::shared_ptr<sf::RenderWindow> mRenderWin;
	boost::shared_ptr<sf::View> mView;
	sf::Shader mShader;

	ResourceManager mResourceManager;
	World mWorld;

	sf::Clock mClock;
	sf::Text mSubtext;

	bool mIsInEditorMode;

	int mNextId;

	int mWorldNumber;
	int mWorldHearts;

	bool mDebugGrid;
	sf::Texture mViewBorder;
	sf::Music mMusic;
};

#endif
