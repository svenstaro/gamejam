#include "GameState.hpp"

#include <Core/ResourceManager.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Utils/Random.hpp>

#include <OgreProcedural.h>

#include <OgreFontManager.h>

#include "AircraftComponent.hpp"
#include "BotControlComponent.hpp"
#include "PlayerControlComponent.hpp"
#include "WorldComponent.hpp"

void GameState::OnInitialize() {
    // setup scene
    mGameScene = AddScene(new dt::Scene("gamescene"));

    dt::Random::Initialize();

    OgreProcedural::Root::getInstance()->sceneManager = mGameScene->GetSceneManager();
    dt::DisplayManager::Get()->SetWindowSize(1024, 768);
    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
    dt::ResourceManager::Get()->AddResourceLocation("particle/","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");
    dt::InputManager::Get()->SetMouseCursorMode(dt::InputManager::SYSTEM);

    OgreProcedural::PlaneGenerator().setSizeX(20.0).setSizeY(20.f).realizeMesh("background");
    OgreProcedural::BoxGenerator().setSizeX(4.0).setSizeY(4.f).setSizeZ(0.1f).realizeMesh("ship_box");
    OgreProcedural::SphereGenerator().setNumRings(4).setNumSegments(8).setRadius(0.1).realizeMesh("cannon_ball");

    dt::Node* camera_node = mGameScene->AddChildNode(new dt::Node("camera_node"));
    camera_node->SetPosition(Ogre::Vector3(0, 0, 30));
    mCamera = camera_node->AddComponent(new dt::CameraComponent("camera"));
    mCamera->LookAt(Ogre::Vector3(0, 0, 0));

    dt::Node* background_node = mGameScene->AddChildNode(new dt::Node("background_node"));
    background_node->SetPosition(0,0,-10);
    background_node->AddComponent(new dt::MeshComponent("background", "BackgroundMesh", "Background"));

    mPlayerAircraft = mGameScene->AddChildNode(new dt::Node("player_aircraft"));
    mPlayerAircraft->SetPosition(-10, 0, 0);
    mPlayerAircraftComponent = mPlayerAircraft->AddComponent(new AircraftComponent(GOOD, "aircraft"));
    mPlayerAircraft->AddComponent(new PlayerControlComponent("aircraft", mPlayerAircraftComponent->GetCannonNode()->GetName(), "control"));

    // an enemy
    dt::Node* enemy_node = mGameScene->AddChildNode(new dt::Node());
    enemy_node->SetPosition(10, 0, 0);
    mEnemyAircraftComponent = enemy_node->AddComponent(new AircraftComponent(EVIL, "aircraft"));
    enemy_node->AddComponent(new BotControlComponent("aircraft", mEnemyAircraftComponent->GetCannonNode()->GetName(), "control"));

    // the world
    dt::Node* world_node = mGameScene->AddChildNode(new dt::Node());
    WorldComponent* world = new WorldComponent();
    world->AddPoint(Ogre::Vector3(17, 12, 0));
    world->AddPoint(Ogre::Vector3(17, -12, 0));
    world->AddPoint(Ogre::Vector3(-17, -12, 0));
    world->AddPoint(Ogre::Vector3(-17, 12, 0));
    world_node->AddComponent(world);

    dt::Node* node;

    node = mGameScene->AddChildNode(new dt::Node());
    node->SetPosition(-14, -10, 0);
    mPlayerHealthDisplay = node->AddComponent(new dt::TextComponent("You: 100", "health"));
    mPlayerHealthDisplay->SetFont("DejaVuSans");
    mPlayerHealthDisplay->SetFontSize(16);

    node = mGameScene->AddChildNode(new dt::Node());
    node->SetPosition(14, -10, 0);
    mEnemyHealthDisplay = node->AddComponent(new dt::TextComponent("Him: 100", "health"));
    mEnemyHealthDisplay->SetFont("DejaVuSans");
    mEnemyHealthDisplay->SetFontSize(16);
}

void GameState::OnDeinitialize() {

}

void GameState::UpdateStateFrame(double simulation_frame_time) {

}
