#include "CannonComponent.hpp"
#include <Graphics/ParticleSystemComponent.hpp>
#include <OgreParticleAffector.h>

CannonComponent::CannonComponent(const QString& name)
    : dt::Component(name) {}

void CannonComponent::OnCreate() {
    mBaseGraphics = GetNode()->AddComponent(new dt::BillboardSetComponent(GetFullName() + "_base_graphics", 1, "cannonbase.png"));
    mBaseGraphics->GetOgreBillboardSet()->setDefaultDimensions(1, 1);

    mBarrelNode  = GetNode()->AddChildNode(new dt::Node(GetFullName() + "_barrel_node"));
    mBarrelGraphics = mBarrelNode->AddComponent(new dt::BillboardSetComponent(GetFullName() + "_barrel_graphics", 1, "cannonbarrel.png"));
    mBarrelGraphics->GetOgreBillboardSet()->setDefaultDimensions(1, 1);
    mBarrelGraphics->SetOrientedSelf();

    //Add the node for particles later on
    mCannonParticles = GetNode()->AddChildNode(new dt::Node("Cannon_Particles"));
}

void CannonComponent::OnDestroy() {
    GetNode()->RemoveComponent(mBaseGraphics->GetName());
    mBarrelNode->RemoveComponent(mBarrelGraphics->GetName());
    GetNode()->RemoveChildNode(mBarrelNode->GetName());
}

void CannonComponent::OnUpdate(double time_diff) {
    Ogre::Vector3 dir = Ogre::Vector3::UNIT_Z;
    dir = GetNode()->GetRotation(dt::Node::SCENE) * dir;
    mBarrelGraphics->GetOgreBillboardSet()->setCommonDirection(dir);
    mBarrelGraphics->GetOgreBillboardSet()->setBillboardType(Ogre::BBT_PERPENDICULAR_COMMON);
}
void CannonComponent::AddParticles()
{
    //Create the particles for the ship
    dt::ParticleSystemComponent* CannonParticleSys = mCannonParticles->AddComponent(new dt::ParticleSystemComponent(GetFullName() + "_ACPsys"));
    CannonParticleSys->SetMaterialName("Test/Particle");
    CannonParticleSys->SetParticleCountLimit(500);
    CannonParticleSys->GetOgreParticleSystem()->setDefaultDimensions(1.5, 1.5);

    Ogre::ParticleEmitter* cemitter = CannonParticleSys->AddEmitter("emit1", "Point");
    cemitter->setAngle(Ogre::Degree(20));
    cemitter->setColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f), Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    cemitter->setEmissionRate(50);
    cemitter->setParticleVelocity(-1.5f, 1.5f);
    cemitter->setTimeToLive(0.2f, 0.75f);

    CannonParticleSys->AddScalerAffector("scaler", -1.0);
    CannonParticleSys->AddLinearForceAffector("force", Ogre::Vector3(0, 1, 0));

    Ogre::ParticleAffector* caffector = CannonParticleSys->AddAffector("colour_interpolator", "ColourInterpolator");
    caffector->setParameter("time0", "0");
    caffector->setParameter("colour0", "0.5 0.5 0.5 1");
    caffector->setParameter("time1", "0.5");
    caffector->setParameter("colour1", "0.5 0.5 0.5 1");
    caffector->setParameter("time2", "1");
    caffector->setParameter("colour2", "0.6 0.6 0.6 0");
}
void CannonComponent::SetParticlesOn(bool pon)
{
    if(pon)
    {
        AddParticles();
    }
    else
    {
        mCannonParticles->RemoveComponent("Cannon_Particles");
    }
}
void CannonComponent::OnSerialize(dt::IOPacket& packet) {}
