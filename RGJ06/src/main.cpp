#include "GameApp.hpp"

int main(int argc, char* argv[]) {
    GameApp& gameapp = GameApp::get_mutable_instance();
    gameapp.Initialize();
    gameapp.Run();

    return 0;
}
