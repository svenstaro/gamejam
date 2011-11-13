#include "PlayerControlComponent.hpp"

#include "AircraftComponent.hpp"

#include <Graphics/DisplayManager.hpp>
#include <Input/InputManager.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

PlayerControlComponent::PlayerControlComponent(QString aircraft_component_name, QString cannon_node_name, QString name)
    : dt::Component(name),
      mAircraftComponentName(aircraft_component_name),
      mCannonNodeName(cannon_node_name) {}

void PlayerControlComponent::OnCreate() {
    connect(dt::InputManager::Get(), SIGNAL(sMouseMoved(const OIS::MouseEvent&)), this, SLOT(MouseMoved(const OIS::MouseEvent&)));
}

void PlayerControlComponent::OnDestroy() {
    dt::InputManager::Get()->disconnect(this);
}

void PlayerControlComponent::OnUpdate(double time_diff) {
    if(IsEnabled()) {
        if(mAircraftComponentName == "") {
            dt::Logger::Get().Error("Cannot control aircraft: no aircraft component set.");
            return;
        }

        float x = 0, y = 0;

        if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_W))
            y = 1;
        if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_S))
            y = -1;

        if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_A))
            x = -1;
        if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_D))
            x = 1;

        AircraftComponent* aircraft = GetNode()->FindComponent<AircraftComponent>(mAircraftComponentName);
        aircraft->GetPhysicsBody()->SetCentralForce(btVector3(x, y, 0) * 10);
    }
}

void PlayerControlComponent::OnSerialize(dt::IOPacket& packet) {
    packet.Stream(mAircraftComponentName, "aircraft_component");
}

void PlayerControlComponent::MouseMoved(const OIS::MouseEvent &arg) {
    float x = arg.state.X.abs * 1.f / dt::DisplayManager::Get()->GetRenderWindow()->getWidth();
    float y = arg.state.Y.abs * 1.f / dt::DisplayManager::Get()->GetRenderWindow()->getHeight();
    Ogre::Ray ray = dt::DisplayManager::Get()->GetMainCamera()->GetCameraToViewportRay(x, y);

    std::pair<bool, Ogre::Real> result = ray.intersects(Ogre::Plane(Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO));

    if(result.first) {
        AircraftComponent* aircraft = GetNode()->FindComponent<AircraftComponent>(mAircraftComponentName);
        dt::Node* cannon = GetNode()->FindChildNode(mCannonNodeName);

        Ogre::Vector3 mouse_point = ray.getPoint(result.second);
        mouse_point -= cannon->GetPosition(dt::Node::SCENE);
        mouse_point.normalise();

        aircraft->SetTargetAngle(Ogre::Vector3::UNIT_X.getRotationTo(mouse_point, Ogre::Vector3::UNIT_Z).getRoll());
    }
}
