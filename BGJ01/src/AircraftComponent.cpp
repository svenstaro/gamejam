#include "AircraftComponent.hpp"

#include "CannonBallComponent.hpp"

#include <Scene/Scene.hpp>

#include <OgreProcedural.h>

AircraftComponent::AircraftComponent(Party party, const QString& name)
    : dt::Component(name),
      mParty(party),
      mShootingCooldown(0) {}

void AircraftComponent::OnCreate() {
    mShipGraphics = GetNode()->AddComponent(new dt::BillboardSetComponent(GetFullName() + "_ship_image", 1, "playership.png"));
    mShipGraphics->GetOgreBillboardSet()->setDefaultDimensions(4.f, 4.f);

    mShipMesh = GetNode()->AddComponent(new dt::MeshComponent("ship_box", "Invisible", GetFullName() + "_ship_mesh"));
    mShipMesh->Disable(); // hide the mesh

    mPhysicsBody = GetNode()->AddComponent(new dt::PhysicsBodyComponent(GetFullName() + "_ship_mesh", GetFullName() + "_physics_body"));
    mPhysicsBody->SetRestrictMovement(btVector3(1, 1, 0));
    mPhysicsBody->SetRestrictRotation(btVector3(0, 0, 0));
    mPhysicsBody->DisableSleep(true);
    mPhysicsBody->SetDampingAmount(0.75, 0.5);
    mPhysicsBody->SetCollisionGroup(short(mParty));
    mPhysicsBody->SetCollisionMask(short(mParty));
    mPhysicsBody->SetGravity(btVector3(0, 0, 0));

    // create the cannon
    mCannonNode = GetNode()->AddChildNode(new dt::Node(GetFullName() +  "_cannon_node"));
    mCannonNode->SetPosition(0.75, -0.70, 0);
    mCannonComponent = mCannonNode->AddComponent(new CannonComponent(GetFullName() + "_cannon"));
}

void AircraftComponent::OnDestroy() {
    GetNode()->RemoveComponent("ship_image");
    GetNode()->RemoveComponent("ship_mesh");
    GetNode()->RemoveComponent("physics_body");
}

void AircraftComponent::OnEnable() {}

void AircraftComponent::OnDisable() {}

void AircraftComponent::OnUpdate(double time_diff) {
    // rotate the cannon to mTargetAngle
    Ogre::Quaternion q;
    q.FromAngleAxis(mTargetAngle, Ogre::Vector3::UNIT_Z);
    mCannonNode->SetRotation(q);

    if(mShootingCooldown >= 0) {
        mShootingCooldown -= time_diff;
    }
}

void AircraftComponent::OnSerialize(dt::IOPacket& packet) {}

dt::PhysicsBodyComponent* AircraftComponent::GetPhysicsBody() {
    return mPhysicsBody;
}

void AircraftComponent::SetTargetAngle(Ogre::Radian angle) {
    mTargetAngle = angle;
}

void AircraftComponent::Shoot() {
    if(mShootingCooldown > 0)
        return;

    dt::Node* ball_node = GetNode()->GetScene()->AddChildNode(new dt::Node());
    ball_node->SetPosition(mCannonNode->GetPosition(dt::Node::SCENE));
    ball_node->AddComponent(new CannonBallComponent(mParty, "ball"))->SetDirection(mTargetAngle);

    mShootingCooldown = 1.f;
}

dt::Node* AircraftComponent::GetCannonNode() {
    return mCannonNode;
}
