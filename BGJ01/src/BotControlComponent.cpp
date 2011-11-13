#include "BotControlComponent.hpp"

#include "AircraftComponent.hpp"

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Utils/Random.hpp>

BotControlComponent::BotControlComponent(QString aircraft_component_name, QString cannon_component_name, QString name)
    : dt::Component(name),
      mAircraftComponentName(aircraft_component_name),
      mCannonNodeName(cannon_component_name) {}

void BotControlComponent::OnCreate() {

}

void BotControlComponent::OnDestroy() {

}

void BotControlComponent::OnUpdate(double time_diff) {
    if(IsEnabled()) {
        if(mAircraftComponentName == "") {
            dt::Logger::Get().Error("Cannot control aircraft: no aircraft component set.");
            return;
        }

        Ogre::Vector3 diff = mTarget - GetNode()->GetPosition(dt::Node::SCENE);
        if(diff.length() < 1) {
            mTarget = Ogre::Vector3(dt::Random::Get(-15.f, 15.f), dt::Random::Get(-10.f, 10.f), 0);
            diff = mTarget - GetNode()->GetPosition(dt::Node::SCENE);
        }
        diff.normalise();

        AircraftComponent* aircraft = GetNode()->FindComponent<AircraftComponent>(mAircraftComponentName);
        aircraft->GetPhysicsBody()->SetCentralForce(btVector3(diff.x, diff.y, 0) * 14);

        dt::Node* player = GetNode()->GetScene()->FindChildNode("player_aircraft");
        //target player
        diff = player->GetPosition(dt::Node::SCENE) - GetNode()->GetPosition(dt::Node::SCENE);
        aircraft->SetTargetAngle(Ogre::Vector3::UNIT_X.getRotationTo(diff, Ogre::Vector3::UNIT_Z).getRoll());

        if(diff.length() < 15)
            aircraft->Shoot();
    }
}

void BotControlComponent::OnSerialize(dt::IOPacket& packet) {}
