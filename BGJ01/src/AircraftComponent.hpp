#ifndef _AIRCRAFTCOMPONENT
#define _AIRCRAFTCOMPONENT

#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

#include <Graphics/BillboardSetComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

class AircraftComponent : public dt::Component {
    Q_OBJECT
public:
    AircraftComponent(QString name);
    void OnCreate();
    void OnDestroy();
    void OnEnable();
    void OnDisable();
    void OnUpdate(double time_diff);
    void OnSerialize(dt::IOPacket& packet);

    dt::PhysicsBodyComponent* GetPhysicsBody();

    void SetTargetAngle(float angle);

private:
    dt::BillboardSetComponent* mShipGraphics;
    dt::MeshComponent* mShipMesh;
    dt::PhysicsBodyComponent* mPhysicsBody;

    float mTargetAngle;
};

#endif
