#include "CannonComponent.hpp"

CannonComponent::CannonComponent(const QString& name)
    : dt::Component(name) {}

void CannonComponent::OnCreate() {
    mBaseGraphics = GetNode()->AddComponent(new dt::BillboardSetComponent(GetFullName() + "_base_graphics", 1, "cannonbase.png"));
    mBaseGraphics->GetOgreBillboardSet()->setDefaultDimensions(1, 1);

    mBarrelNode  = GetNode()->AddChildNode(new dt::Node(GetFullName() + "_barrel_node"));
    mBarrelGraphics = mBarrelNode->AddComponent(new dt::BillboardSetComponent(GetFullName() + "_barrel_graphics", 1, "cannonbarrel.png"));
    mBarrelGraphics->GetOgreBillboardSet()->setDefaultDimensions(1, 1);
    mBarrelGraphics->SetOrientedSelf();
}

void CannonComponent::OnDestroy() {
    GetNode()->RemoveComponent(mBaseGraphics->GetName());
    mBarrelNode->RemoveComponent(mBarrelGraphics->GetName());
    GetNode()->RemoveChildNode(mBarrelNode->GetName());
}

void CannonComponent::OnUpdate(double time_diff) {
    Ogre::Vector3 dir = Ogre::Vector3::UNIT_Z;
    dir = GetNode()->GetRotation(dt::Node::SCENE) * dir;
    mBarrelGraphics->GetOgreBillboardSet()->setCommonDirection(dir);
    mBarrelGraphics->GetOgreBillboardSet()->setBillboardType(Ogre::BBT_PERPENDICULAR_COMMON);
}

void CannonComponent::OnSerialize(dt::IOPacket& packet) {}
