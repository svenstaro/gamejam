#include "CannonBallComponent.hpp"
#include <Graphics/ParticleSystemComponent.hpp>
#include <OgreParticleAffector.h>

CannonBallComponent::CannonBallComponent(Party party, AircraftComponent* aircraft, const QString& name)
    : dt::Component(name),
      mParty(party),
      mAircraft(aircraft) {}

void CannonBallComponent::OnCreate() {
    mMesh = GetNode()->AddComponent(new dt::MeshComponent("cannon_ball", "CannonBall", GetFullName() + "_mesh"));
    mPhysicsBody = new dt::PhysicsBodyComponent(GetFullName() + "_mesh", GetFullName() + "_physics_body");
    mPhysicsBody->SetCollisionShapeType(dt::PhysicsBodyComponent::SPHERE);
    GetNode()->AddComponent(mPhysicsBody);
    mPhysicsBody->SetRestrictMovement(btVector3(1, 1, 0));
    mPhysicsBody->DisableSleep(true); // TODO: really necessary?
    mPhysicsBody->SetDampingAmount(0.15, 0.0); // TODO: Play with this
    mPhysicsBody->SetMass(0.1);
    mPhysicsBody->SetCollisionGroup(short(OppositeParty(mParty)));
    mPhysicsBody->SetCollisionMask(short(OppositeParty(mParty)));

    //Add particles to the ball
    mBallParticles = GetNode()->AddChildNode(new dt::Node("Cannon_Ball_Particles"));
    //mBallParticles->SetPosition(GetNode()->GetPosition());
    SetParticlesOn(true);
}

void CannonBallComponent::SetDirection(Ogre::Radian angle) {
    // apply impulse
    Ogre::Quaternion q;
    q.FromAngleAxis(angle, Ogre::Vector3::UNIT_Z);
    Ogre::Vector3 imp = q * Ogre::Vector3::UNIT_X;
    mPhysicsBody->ApplyCentralImpulse(btVector3(imp.x, imp.y, imp.z) * 1.5);
}

void CannonBallComponent::OnDestroy() {
    GetNode()->RemoveComponent(mMesh->GetName());
    GetNode()->RemoveComponent(mPhysicsBody->GetName());
}
void CannonBallComponent::AddParticles()
{
    //Create the particles for the ship
    dt::ParticleSystemComponent* BallParticleSys = mBallParticles->AddComponent(new dt::ParticleSystemComponent(GetFullName() + "_BPsys"));
    BallParticleSys->SetMaterialName("Test/Particle");
    BallParticleSys->SetParticleCountLimit(500);
    BallParticleSys->GetOgreParticleSystem()->setDefaultDimensions(1, 1);

    Ogre::ParticleEmitter* bemitter = BallParticleSys->AddEmitter("emit1", "Point");
    bemitter->setPosition(Ogre::Vector3(0,0,0.5));
    bemitter->setAngle(Ogre::Degree(180));
    bemitter->setColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f), Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    bemitter->setEmissionRate(30);
    bemitter->setParticleVelocity(-1.5f, 1.5f);
    bemitter->setTimeToLive(0.2f, 0.75f);

    BallParticleSys->AddScalerAffector("scaler", -1.0);

    Ogre::ParticleAffector* baffector = BallParticleSys->AddAffector("colour_interpolator", "ColourInterpolator");
    baffector->setParameter("time0", "0");
    baffector->setParameter("colour0", "0.5 0.5 0.5 1");
    baffector->setParameter("time1", "0.5");
    baffector->setParameter("colour1", "0.5 0.5 0.5 1");
    baffector->setParameter("time2", "1");
    baffector->setParameter("colour2", "0.6 0.6 0.6 0");
}
void CannonBallComponent::SetParticlesOn(bool pon)
{
    if(pon)
    {
        AddParticles();
    }
    else
    {
        mBallParticles->RemoveComponent("Cannon_Ball_Particles");
    }
}
void CannonBallComponent::OnSerialize(dt::IOPacket& packet) {}

void CannonBallComponent::OnUpdate(double time_diff) {
    if(GetNode()->GetPosition(dt::Node::SCENE).y < -11.9) {
        GetNode()->RemoveComponent(GetName());
    }
}

AircraftComponent* CannonBallComponent::GetAircraft() {
    return mAircraft;
}
