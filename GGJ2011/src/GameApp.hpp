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

#include "AnimatedSprite.hpp"

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

	void LoadWorld(const boost::filesystem::path& data_path);

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

	void SetSubtext(const std::string& subtext);

	World* const GetWorldPtr();
	ResourceManager* const GetResourceManagerPtr();
	boost::shared_ptr<sf::RenderWindow> GetRenderWindowPtr();

    void ShowCredits();
private:
	boost::shared_ptr<sf::RenderWindow> mRenderWin;
	boost::shared_ptr<sf::View> mView;
	sf::Shader mShader;

	ResourceManager mResourceManager;
	World mWorld;

	sf::Clock mClock;
	float mTotalTime;
	sf::Text mSubtext;

	AppMode mAppMode;

	int mNextId;

	bool mDebugGrid;
	sf::Music mMusic;
	sf::Sprite mCursor;
	float mCursorRotation;

	AnimatedSprite mBusy;

    bool mCreditsActive;
    float mTimeSinceCreditsActive;
};

#endif
