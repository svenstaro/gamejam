#ifndef _CANNONBALLCOMPONENT
#define _CANNONBALLCOMPONENT

#include "Enums.hpp"

#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

class CannonBallComponent : public dt::Component {
    Q_OBJECT
public:
    CannonBallComponent(Party party, const QString& name);
    void OnCreate();
    void OnDestroy();
    void OnSerialize(dt::IOPacket& packet);

    void SetDirection(Ogre::Radian angle);

private:
    dt::MeshComponent* mMesh;
    dt::PhysicsBodyComponent* mPhysicsBody;
    Party mParty;

};

#endif
