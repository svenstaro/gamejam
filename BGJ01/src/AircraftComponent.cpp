#include "AircraftComponent.hpp"

#include "CannonBallComponent.hpp"
#include <Graphics/ParticleSystemComponent.hpp>
#include <OgreParticleAffector.h>
#include <Scene/Scene.hpp>

#include <OgreProcedural.h>

AircraftComponent::AircraftComponent(Party party, const QString& name)
    : dt::Component(name),
      mParty(party) {}

void AircraftComponent::OnCreate() {
    mShipGraphics = GetNode()->AddComponent(new dt::BillboardSetComponent(GetFullName() + "_ship_image", 1, "playership.png"));
    mShipGraphics->GetOgreBillboardSet()->setDefaultDimensions(4.f, 4.f);

    mShipMesh = GetNode()->AddComponent(new dt::MeshComponent("ship_box", "Invisible", GetFullName() + "_ship_mesh"));
    mShipMesh->Disable(); // hide the mesh

    mPhysicsBody = GetNode()->AddComponent(new dt::PhysicsBodyComponent(GetFullName() + "_ship_mesh", GetFullName() + "_physics_body"));
    mPhysicsBody->SetTwoDimensional(true);
    mPhysicsBody->DisableDeactivation(true);
    mPhysicsBody->SetDampingAmount(0.75, 0.5);
    mPhysicsBody->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0));
    mPhysicsBody->SetCollisionGroup(short(mParty));
    mPhysicsBody->SetCollisionMask(short(mParty));
    mPhysicsBody->SetGravity(btVector3(0, 0, 0));

    // create the cannon
    mCannonNode = GetNode()->AddChildNode(new dt::Node(GetFullName() +  "_cannon_node"));
    mCannonNode->SetPosition(0.75, -0.70, 0);
    mCannonComponent = mCannonNode->AddComponent(new CannonComponent(GetFullName() + "_cannon"));

    //Add The ship's particles
    mAircraftParticles = GetNode()->AddChildNode(new dt::Node("Aircraft_Particles"));
    mAircraftParticles->SetPosition(Ogre::Vector3(-2, 0.2, 0));
    SetParticlesOn(true);
}

void AircraftComponent::OnDestroy() {
    GetNode()->RemoveComponent("ship_image");
    GetNode()->RemoveComponent("ship_mesh");
    GetNode()->RemoveComponent("physics_body");
    GetNode()->RemoveChildNode("mAircraftParticles");
}

void AircraftComponent::OnEnable() {}

void AircraftComponent::OnDisable() {}

void AircraftComponent::OnUpdate(double time_diff) {
    // rotate the cannon to mTargetAngle
    Ogre::Quaternion q;
    q.FromAngleAxis(mTargetAngle, Ogre::Vector3::UNIT_Z);
    mCannonNode->SetRotation(q);
}

void AircraftComponent::OnSerialize(dt::IOPacket& packet) {}

dt::PhysicsBodyComponent* AircraftComponent::GetPhysicsBody() {
    return mPhysicsBody;
}

void AircraftComponent::SetTargetAngle(Ogre::Radian angle) {
    mTargetAngle = angle;
}
void AircraftComponent::AddParticles()
{
    //Create the particles for the ship
    dt::ParticleSystemComponent* AircraftParticleSys = mAircraftParticles->AddComponent(new dt::ParticleSystemComponent("ACPsys"));
    AircraftParticleSys->SetMaterialName("Test/Particle");
    AircraftParticleSys->SetParticleCountLimit(500);
    AircraftParticleSys->GetOgreParticleSystem()->setDefaultDimensions(1.5, 1.5);

    Ogre::ParticleEmitter* emitter = AircraftParticleSys->AddEmitter("emit1", "Point");
    emitter->setAngle(Ogre::Degree(20));
    emitter->setColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f), Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    emitter->setEmissionRate(50);
    emitter->setParticleVelocity(-1.5f, 1.5f);
    emitter->setTimeToLive(0.2f, 0.75f);

    AircraftParticleSys->AddScalerAffector("scaler", -1.0);

    Ogre::ParticleAffector* affector = AircraftParticleSys->AddAffector("colour_interpolator", "ColourInterpolator");
    affector->setParameter("time0", "0");
    affector->setParameter("colour0", "0.5 0.5 0.5 1");
    affector->setParameter("time1", "0.5");
    affector->setParameter("colour1", "0.5 0.5 0.5 1");
    affector->setParameter("time2", "1");
    affector->setParameter("colour2", "0.6 0.6 0.6 0");
}
void AircraftComponent::SetParticlesOn(bool pon)
{
    if(pon)
    {
        AddParticles();
    }
    else
    {
        mAircraftParticles->RemoveComponent("Aircraft_Particles");
    }
}
void AircraftComponent::Shoot() {
    dt::Node* ball_node = GetNode()->GetScene()->AddChildNode(new dt::Node());
    ball_node->SetPosition(mCannonNode->GetPosition(dt::Node::SCENE));
    ball_node->AddComponent(new CannonBallComponent(mParty, "ball"))->SetDirection(mTargetAngle);
}

dt::Node* AircraftComponent::GetCannonNode() {
    return mCannonNode;
}
