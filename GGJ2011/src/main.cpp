#include "GameApp.hpp"

int main(int argc, char* argv[]) {
	GameApp& app = GameApp::get_mutable_instance();
	app.Init(argv);
	app.Run();
	return 0;
}
