#ifndef _WORLDCOMPONENT
#define _WORLDCOMPONENT

#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

#include <Graphics/BillboardSetComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>


class WorldComponent : public dt::Component {
    Q_OBJECT
public:
    WorldComponent(const QString& name = "");

    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);
    void OnSerialize(dt::IOPacket& packet);

    void AddPoint(Ogre::Vector3 point);

private:
    std::vector<Ogre::Vector3> mPoints;
    dt::MeshComponent* mMeshComponent;
    dt::PhysicsBodyComponent* mPhysicsBodyComponent;

};

#endif
