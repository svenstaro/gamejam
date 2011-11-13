#include "WorldComponent.hpp"

#include <OgreProcedural.h>

WorldComponent::WorldComponent(const QString& name)
    : dt::Component(name) {

    AddPoint(Ogre::Vector3(10, 10, 0));
    AddPoint(Ogre::Vector3(15, 0, 0));
    AddPoint(Ogre::Vector3(10, -10, 0));
    AddPoint(Ogre::Vector3(0, -15, 0));
    AddPoint(Ogre::Vector3(-10, -20, 0));
    AddPoint(Ogre::Vector3(-10, -3, 0));
    AddPoint(Ogre::Vector3(-11, 1, 0));
    AddPoint(Ogre::Vector3(-2, 19, 0));
}

void WorldComponent::OnCreate() {
    // create the mesh
    if(mPoints.size() < 3)
        return;

    OgreProcedural::Shape s;
    for(auto iter = mPoints.begin(); iter != mPoints.end(); ++iter) {
        s.addPoint(iter->x, iter->y);
    }
    s.close();

    OgreProcedural::Path p;
    p.addPoint(Ogre::Vector3(0, 0, -1));
    p.addPoint(Ogre::Vector3(0, 0, 1));
    // OgreProcedural::Path pa = p.realizePath();

    OgreProcedural::Extruder e;
    e.setExtrusionPath(&p).setShapeToExtrude(&s).realizeMesh(GetFullName().toStdString() + "_mesh_type");

    mMeshComponent = GetNode()->AddComponent(new dt::MeshComponent(GetFullName() + "_mesh_type", "Invisible", GetFullName() + "_mesh"));
    // mMeshComponent->Disable(); // hide mesh
    mPhysicsBodyComponent = new dt::PhysicsBodyComponent(mMeshComponent->GetName(), GetFullName() + "_physics_body");
    mPhysicsBodyComponent->SetCollisionShapeType(dt::PhysicsBodyComponent::TRIMESH);
    GetNode()->AddComponent(mPhysicsBodyComponent);
    mPhysicsBodyComponent->SetRestrictMovement(btVector3(1, 1, 0));
    mPhysicsBodyComponent->SetRestrictRotation(btVector3(0, 0, 0));
    mPhysicsBodyComponent->SetMass(0);
}

void WorldComponent::OnDestroy() {}

void WorldComponent::OnUpdate(double time_diff) {}

void WorldComponent::OnSerialize(dt::IOPacket& packet) {
    /*// Components
    uint32_t count = packet.BeginList(mPoints.size(), "points");

    if(packet.GetDirection() == dt::IOPacket::SERIALIZE) {
        // serialize
        for(auto iter = mPoints.begin(); iter != mPoints.end(); ++iter) {
            packet.BeginObject();
            Ogre::Vector3 v = *iter;
            packet.Stream(v, "point");
            packet.EndObject();
        }
    } else {
        mPoints.clear();
        for(uint32_t i = 0; i < count; ++i) {
            packet.BeginObject();
            Ogre::Vector3 p;
            packet.Stream(p, "point");
            AddPoint(p);
            packet.EndObject();
        }
    }
    packet.EndList();*/
}

void WorldComponent::AddPoint(Ogre::Vector3 point) {
    mPoints.push_back(point);
}
