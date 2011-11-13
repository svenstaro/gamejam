#ifndef _CANNONCOMPONENT
#define _CANNONCOMPONENT

#include <Scene/Component.hpp>
#include <Scene/Node.hpp>

#include <Graphics/BillboardSetComponent.hpp>

class CannonComponent : public dt::Component {
    Q_OBJECT
public:
    CannonComponent(const QString& name);
    void OnCreate();
    void OnDestroy();
    void OnUpdate(double time_diff);
    void OnSerialize(dt::IOPacket& packet);

private:
    dt::BillboardSetComponent* mBaseGraphics;
    dt::Node* mBarrelNode;
    dt::BillboardSetComponent* mBarrelGraphics;

};

#endif
