#include "PlayerControlComponent.hpp"

#include "AircraftComponent.hpp"

#include <Input/InputManager.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

PlayerControlComponent::PlayerControlComponent(QString aircraft_component_name, QString name)
    : dt::Component(name),
      mAircraftComponentName(aircraft_component_name) {}

void PlayerControlComponent::OnCreate() {

}

void PlayerControlComponent::OnDestroy() {

}

void PlayerControlComponent::OnUpdate(double time_diff) {
    if(IsEnabled()) {
        if(mAircraftComponentName == "") {
            dt::Logger::Get().Error("Cannot control aircraft: no aircraft component set.");
            return;
        }

        /*float x = 0, y = 0;

        if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_W))
            y = 1;
        if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_S))
            y = -1;

        if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_A))
            x = -1;
        if(dt::InputManager::Get()->GetKeyboard()->isKeyDown(OIS::KC_D))
            x = 1;

        AircraftComponent* aircraft = GetNode()->FindComponent<AircraftComponent>(mAircraftComponentName);
        aircraft->GetPhysicsBody()->SetForce(btVector3(x, y, 0) * 10); */
    }
}

void PlayerControlComponent::OnSerialize(dt::IOPacket& packet) {
    packet.Stream(mAircraftComponentName, "aircraft_component");
}
