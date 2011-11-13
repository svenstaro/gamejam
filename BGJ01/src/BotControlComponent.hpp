#ifndef _BOTCONTROLCOMPONENT
#define _BOTCONTROLCOMPONENT

#include <Scene/Component.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Input/InputManager.hpp>

class BotControlComponent : public dt::Component {
    Q_OBJECT
public:
    BotControlComponent(QString aircraft_component_name, QString cannon_component_name, QString name);
    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);
    void OnSerialize(dt::IOPacket& packet);

private:
    QString mAircraftComponentName;
    QString mCannonNodeName;

    Ogre::Vector3 mTarget;

};

#endif
