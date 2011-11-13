#ifndef _CANNONBALLCOMPONENT
#define _CANNONBALLCOMPONENT

#include "Enums.hpp"

#include "AircraftComponent.hpp"

#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

class CannonBallComponent : public dt::Component {
    Q_OBJECT
public:
    CannonBallComponent(Party party, AircraftComponent* aircraft, const QString& name);
    void OnCreate();
    void OnDestroy();
    void OnSerialize(dt::IOPacket& packet);
    void OnUpdate(double time_diff);

    void SetDirection(Ogre::Radian angle);

    AircraftComponent* GetAircraft();

private:
    dt::MeshComponent* mMesh;
    dt::PhysicsBodyComponent* mPhysicsBody;
    Party mParty;
    AircraftComponent* mAircraft;

};

#endif
