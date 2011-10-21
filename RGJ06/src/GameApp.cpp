#include "GameApp.hpp"

GameApp::GameApp() {

}

void GameApp::Initialize() {
    mRenderWindow.Create(sf::VideoMode(800, 600, 32), "EnergyMaster", sf::Style::Close | sf::Style::Titlebar, sf::ContextSettings(24, 8, 4));
    std::cout << "Antialiasing level: " << mRenderWindow.GetSettings().AntialiasingLevel << std::endl;
    mRenderWindow.ShowMouseCursor(false);

    mResourceManager.AddTexture("../gfx/",   "particle.png",   20,  20);
    mResourceManager.AddTexture("../gfx/",   "blob.png",       100, 100);
    mResourceManager.AddTexture("../gfx/",   "enemy.png",      100, 100);
    mResourceManager.AddTexture("../gfx/",   "player.png",     200, 200);
    mResourceManager.AddTexture("../gfx/",   "photon.png",     200, 200);
    mResourceManager.AddTexture("../gfx/",   "background.png", 800, 600);
    mResourceManager.AddTexture("../gfx/",   "foreground.png", 800, 600);

    mResourceManager.AddSoundBuffer("../snd/", "death.ogg");
    mResourceManager.AddSoundBuffer("../snd/", "killenemy.ogg");
    mResourceManager.AddSoundBuffer("../snd/", "select.ogg");
    mResourceManager.AddSoundBuffer("../snd/", "spawn.ogg");

    mMusic.OpenFromFile("../snd/tha-live_fire.ogg");
    mMusic.SetLoop(true);
    mMusic.Play();

    mWorld.Initialize();
    mClock.Reset();
}

void GameApp::Run() {
    const float fps = 60.f;
    const float dt = 1/fps;
    float time_budget = 0.f;

    while(mRenderWindow.IsOpened()) {
        HandleEvents();

        float time_delta = mClock.GetElapsedTime();
        time_delta /= 1000;
        mClock.Reset();

        time_budget += time_delta;
        while(time_budget >= dt) {
            Update(dt);
            time_budget -= dt;
        }

        Draw();
    }
}

void GameApp::HandleEvents() {
    sf::Event event;
    while(mRenderWindow.PollEvent(event)) {
        if((event.Type == sf::Event::Closed) ||
                (event.Type == sf::Event::KeyPressed &&
                 event.Key.Code == sf::Keyboard::Escape)) {
            mRenderWindow.Close();
        }

        //handle event 
        mWorld.HandleEvent(event);
    }
}

void GameApp::Update(float frame_time) {
    mWorld.Update(frame_time);
}

void GameApp::Draw() {
    mRenderWindow.Clear();
    mWorld.Draw(&mRenderWindow);
    mRenderWindow.Display();
}

ResourceManager* const GameApp::GetResourceManagerPtr() {
    return &mResourceManager;
}
    
World& GameApp::GetWorld() {
    return mWorld;
}

float GameApp::Random(float min, float max) {
    return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
}
int GameApp::Random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

