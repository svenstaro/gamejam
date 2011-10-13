#include "GameApp.hpp"

GameApp::GameApp() {

}

void GameApp::Initialize() {
    mRenderWindow.Create(sf::VideoMode(800, 600, 32), "EnergyMaster", sf::Style::Close | sf::Style::Titlebar, sf::ContextSettings(24, 8, 4));
    std::cout << "Antialiasing level: " << mRenderWindow.GetSettings().AntialiasingLevel << std::endl;
    mRenderWindow.ShowMouseCursor(false);

    mResourceManager.AddImage("../gfx/",   "particle.png",   20,  20);
    mResourceManager.AddImage("../gfx/",   "blob.png",       100, 100);
    mResourceManager.AddImage("../gfx/",   "enemy.png",      100, 100);
    mResourceManager.AddImage("../gfx/",   "player.png",     200, 200);
    mResourceManager.AddImage("../gfx/",   "photon.png",     200, 200);
    mResourceManager.AddImage("../gfx/",   "background.png", 800, 600);
    mResourceManager.AddImage("../gfx/",   "foreground.png", 800, 600);

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
                 event.Key.Code == sf::Key::Escape)) {
            mRenderWindow.Close();
        }

        //handle event 
        mWorld.HandleEvent(event, mRenderWindow.GetInput());
    }
}

void GameApp::Update(float frame_time) {
    mWorld.Update(frame_time, mRenderWindow.GetInput());
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
