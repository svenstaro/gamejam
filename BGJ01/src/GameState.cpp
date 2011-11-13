#include "GameState.hpp"

#include <Graphics/DisplayManager.hpp>
#include <Core/ResourceManager.hpp>

#include <OgreProcedural.h>

#include <OgreFontManager.h>

#include "AircraftComponent.hpp"
#include "PlayerControlComponent.hpp"

void GameState::OnInitialize() {
    // setup scene
    mGameScene = AddScene(new dt::Scene("gamescene"));

    OgreProcedural::Root::getInstance()->sceneManager = mGameScene->GetSceneManager();
    dt::DisplayManager::Get()->SetWindowSize(1024, 768);
    dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    Ogre::FontManager::getSingleton().load("DejaVuSans", "General");
    dt::InputManager::Get()->SetMouseCursorMode(dt::InputManager::SYSTEM);

    OgreProcedural::PlaneGenerator().setSizeX(20.0).setSizeY(20.f).realizeMesh("background");
    OgreProcedural::BoxGenerator().setSizeX(1.0).setSizeY(1.f).setSizeZ(0.1f).realizeMesh("ship_box");

    dt::Node* camera_node = mGameScene->AddChildNode(new dt::Node("camera_node"));
    camera_node->SetPosition(Ogre::Vector3(0, 0, 30));
    mCamera = camera_node->AddComponent(new dt::CameraComponent("camera"));
    mCamera->LookAt(Ogre::Vector3(0, 0, 0));

    dt::Node* background_node = mGameScene->AddChildNode(new dt::Node("background_node"));
    background_node->SetPosition(0,0,-10);
    background_node->AddComponent(new dt::MeshComponent("background", "BackgroundMesh", "Background"));

    mPlayerAircraft = mGameScene->AddChildNode(new dt::Node("player_aircraft"));
    AircraftComponent* aircraft = mPlayerAircraft->AddComponent(new AircraftComponent("aircraft"));
    mPlayerAircraft->AddComponent(new PlayerControlComponent("aircraft", aircraft->GetCannonNode()->GetName(), "control"));
}

void GameState::OnDeinitialize() {

}

void GameState::UpdateStateFrame(double simulation_frame_time) {

}
