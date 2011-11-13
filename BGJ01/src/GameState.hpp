#ifndef _GAMESTATE
#define _GAMESTATE

#include <Scene/State.hpp>
#include <Graphics/CameraComponent.hpp>

class GameState : public dt::State {
    Q_OBJECT
public:
    void OnInitialize();
    void OnDeinitialize();
    void UpdateStateFrame(double simulation_frame_time);

private:
    dt::Node* mPlayerAircraft;
    dt::Scene* mGameScene;
    dt::CameraComponent* mCamera;

};

#endif
