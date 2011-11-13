#ifndef _AIRCRAFTCOMPONENT
#define _AIRCRAFTCOMPONENT

#include "CannonComponent.hpp"

#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

#include <Graphics/BillboardSetComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

class AircraftComponent : public dt::Component {
    Q_OBJECT
public:
    AircraftComponent(const QString& name);
    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);
    void OnSerialize(dt::IOPacket& packet);

    dt::PhysicsBodyComponent* GetPhysicsBody();

    void SetTargetAngle(Ogre::Radian angle);
    dt::Node* GetCannonNode();

private:
    dt::BillboardSetComponent* mShipGraphics;
    dt::MeshComponent* mShipMesh;
    dt::PhysicsBodyComponent* mPhysicsBody;

    dt::Node* mCannonNode;
    CannonComponent* mCannonComponent;

    Ogre::Radian mTargetAngle;
};

#endif
