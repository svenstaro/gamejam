#ifndef _AIRCRAFTCOMPONENT
#define _AIRCRAFTCOMPONENT

#include "CannonComponent.hpp"
#include "Enums.hpp"

#include <Scene/Component.hpp>
#include <Scene/Node.hpp>
#include <Graphics/BillboardSetComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>


class AircraftComponent : public dt::Component {
    Q_OBJECT
public:
    AircraftComponent(Party party, const QString& name = "");
    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);
    void AddParticles();
    void SetParticlesOn(bool pon);
    void OnSerialize(dt::IOPacket& packet);

    dt::PhysicsBodyComponent* GetPhysicsBody();

    void SetTargetAngle(Ogre::Radian angle);
    void Shoot();
    dt::Node* GetCannonNode();

private:
    dt::BillboardSetComponent* mShipGraphics;
    dt::MeshComponent* mShipMesh;
    dt::PhysicsBodyComponent* mPhysicsBody;
    dt::Node* mAircraftParticles;
    dt::Node* mCannonNode;
    CannonComponent* mCannonComponent;

    Ogre::Radian mTargetAngle;

    Party mParty;
};

#endif
