#ifndef GAMEAPP_HPP
#define GAMEAPP_HPP

#include <boost/serialization/singleton.hpp>

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"
#include "World.hpp"

class GameApp : public boost::serialization::singleton<GameApp> {
public:
    GameApp();
    void Initialize();
    void Run();

    ResourceManager* const GetResourceManagerPtr();
    World& GetWorld();

    static float Random(float min, float max);
    static int Random(int min, int max);

private:
    void HandleEvents();
    void Update(float frame_time);
    void Draw();

    sf::RenderWindow mRenderWindow;
    sf::Clock mClock;
    sf::Music mMusic;

    ResourceManager mResourceManager;
    World mWorld;    
};

#endif
