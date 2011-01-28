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

enum AppMode {
	AM_EDITOR,
	AM_PUZZLE,
	AM_PLAY
};

class GameApp : public boost::serialization::singleton<GameApp> {
public:
	GameApp();
	~GameApp();

	void Init();
	void Run();
	void Quit();

	void LoadWorld();

	void SetAppMode(AppMode mode);
	bool IsEditorMode() const;
	AppMode GetAppMode() const;

    const Vector2D GetWindowSize() const;
	sf::View& GetView();
	void SetGuiPaintingMode(bool guipaint);
	const sf::Input& GetInput() const;
	Vector2D GetMousePosition() const;

	int GetNextId();
	void SetNextId(int id);
	sf::Color GetSpriteColor(const float alpha) const;

	void SetSubtext(const std::string& subtext);

	void SetWorldHearts(int h);
	int GetWorldHearts() const;

	World* const GetWorldPtr();
	ResourceManager* const GetResourceManagerPtr();
	boost::shared_ptr<sf::RenderWindow> GetRenderWindowPtr();
private:
	boost::shared_ptr<sf::RenderWindow> mRenderWin;
	boost::shared_ptr<sf::View> mView;
	sf::Shader mShader;

	ResourceManager mResourceManager;
	World mWorld;

	sf::Clock mClock;
	sf::Text mSubtext;

	AppMode mAppMode;

	int mNextId;

	int mWorldNumber;
	int mWorldHearts;

	bool mDebugGrid;
	sf::Image mViewBorder;
	sf::Music mMusic;
};

#endif
