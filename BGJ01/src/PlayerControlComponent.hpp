#ifndef _PLAYERCONTROLCOMPONENT
#define _PLAYERCONTROLCOMPONENT

#include <Scene/Component.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

class PlayerControlComponent : public dt::Component {
    Q_OBJECT
public:
    PlayerControlComponent(QString aircraft_component_name, QString name);
    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);
    void OnSerialize(dt::IOPacket& packet);

private:
    QString mAircraftComponentName;

};

#endif
