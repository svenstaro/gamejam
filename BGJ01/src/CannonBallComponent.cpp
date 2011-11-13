#include "CannonBallComponent.hpp"

CannonBallComponent::CannonBallComponent(Party party, const QString& name)
    : dt::Component(name),
      mParty(party) {}

void CannonBallComponent::OnCreate() {
    mMesh = GetNode()->AddComponent(new dt::MeshComponent("cannon_ball", "CannonBall", GetFullName() + "_mesh"));
    mPhysicsBody = new dt::PhysicsBodyComponent(GetFullName() + "_mesh", GetFullName() + "_physics_body");
    mPhysicsBody->SetCollisionShapeType(dt::PhysicsBodyComponent::SPHERE);
    GetNode()->AddComponent(mPhysicsBody);
    mPhysicsBody->SetRestrictMovement(btVector3(1, 1, 0));
    mPhysicsBody->DisableSleep(true); // TODO: really necessary?
    mPhysicsBody->SetDampingAmount(0.15, 0.0); // TODO: Play with this
    mPhysicsBody->SetMass(0.1);
    mPhysicsBody->SetCollisionGroup(short(OppositeParty(mParty)));
    mPhysicsBody->SetCollisionMask(short(OppositeParty(mParty)));
}

void CannonBallComponent::SetDirection(Ogre::Radian angle) {
    // apply impulse
    Ogre::Quaternion q;
    q.FromAngleAxis(angle, Ogre::Vector3::UNIT_Z);
    Ogre::Vector3 imp = q * Ogre::Vector3::UNIT_X;
    mPhysicsBody->ApplyCentralImpulse(btVector3(imp.x, imp.y, imp.z) * 1.5);
}

void CannonBallComponent::OnDestroy() {
    GetNode()->RemoveComponent(mMesh->GetName());
    GetNode()->RemoveComponent(mPhysicsBody->GetName());
}

void CannonBallComponent::OnSerialize(dt::IOPacket& packet) {}
