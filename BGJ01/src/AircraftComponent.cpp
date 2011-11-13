#include "AircraftComponent.hpp"

#include <OgreProcedural.h>

AircraftComponent::AircraftComponent(const QString& name)
    : dt::Component(name) {}

void AircraftComponent::OnCreate() {
    mShipGraphics = GetNode()->AddComponent(new dt::BillboardSetComponent("ship_image", 1, "playership.png"));
    mShipGraphics->GetOgreBillboardSet()->setDefaultDimensions(4.f, 4.f);

    mShipMesh = GetNode()->AddComponent(new dt::MeshComponent("ship_box", "ship_box_material", "ship_mesh"));

    mPhysicsBody = GetNode()->AddComponent(new dt::PhysicsBodyComponent("ship_mesh", "physics_body"));
    /*mPhysicsBody->HasForce(true);
    mPhysicsBody->SetTwoDimensional(true);
    mPhysicsBody->SetGravity(btVector3(0, 0, 0));
    mPhysicsBody->DisableDeactivation(true);
    mPhysicsBody->SetDampingAmount(0.75, 0.5);*/

}

void AircraftComponent::OnDestroy() {
    GetNode()->RemoveComponent("ship_image");
    GetNode()->RemoveComponent("ship_mesh");
    GetNode()->RemoveComponent("physics_body");
}

void AircraftComponent::OnEnable() {}

void AircraftComponent::OnDisable() {}

void AircraftComponent::OnUpdate(double time_diff) {
    // TODO: rotate the cannon to mTargetAngle
}

void AircraftComponent::OnSerialize(dt::IOPacket& packet) {}

dt::PhysicsBodyComponent* AircraftComponent::GetPhysicsBody() {
    return mPhysicsBody;
}

void AircraftComponent::SetTargetAngle(float angle) {
    mTargetAngle = angle;
}
