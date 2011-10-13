#include "GameApp.hpp"

int main() {
	GameApp& app = GameApp::get_mutable_instance();
	app.Init();
	app.Run();
	return 0;
}
