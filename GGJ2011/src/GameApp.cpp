#include "GameApp.hpp"

#include "boost/date_time/posix_time/posix_time.hpp"

std::string leadingZeros(int i, int n) {
	std::string s = boost::lexical_cast<std::string>(i);
	while(s.length() < n) {
		s = "0" + s;
	}
	return s;
}

GameApp::GameApp() {
	mNextId = 1300;
	mDebugGrid = false;
	mAppMode = AM_PLAY;
}

GameApp::~GameApp() {}

void GameApp::Init() {
	mRenderWin = boost::shared_ptr<sf::RenderWindow>(new sf::RenderWindow);
	mRenderWin->Create(sf::VideoMode(WIDTH, HEIGHT, 32), "AI and the bomb", sf::Style::Default, sf::ContextSettings(24, 8, 4));
	mRenderWin->SetPosition(sf::VideoMode::GetDesktopMode().Width / 2 - WIDTH / 2,sf::VideoMode::GetDesktopMode().Height / 2 - HEIGHT / 2);
	mRenderWin->EnableVerticalSync(true);

	mView = boost::shared_ptr<sf::View>(new sf::View(sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(WIDTH, HEIGHT)));
	mRenderWin->SetView(*mView.get());

	// load shader
	mRenderWin->SetActive(true);
	// shader is broken ;( disabled
	/*mShader.LoadFromFile("../data/SaturationShader.glsl");
	mShader.SetTexture("tex", sf::Shader::CurrentTexture);
	mShader.SetParameter("new_sat", 0.8f);*/


    // figure out resource path
    boost::filesystem::path data;
    if(boost::filesystem::is_directory("data/")) {
        data = "data/";
    } else if(boost::filesystem::is_directory("../data/")) {
        data = "../data/";
    } else if(boost::filesystem::is_directory("../../data/")) {
        data = "../../data/";
    } else {
		std::cerr << "Didn't find any valid data path." << std::endl;
		exit(1);
    }

	// load resources
	mResourceManager.AddImage(data / "gfx", "box.png", 200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(data / "gfx", "rail.png", 20*METERS_PER_PIXEL, 20*METERS_PER_PIXEL);
	mResourceManager.AddImage(data / "gfx", "magnet_pull.png", 1.f, 1.f);
	mResourceManager.AddImage(data / "gfx", "magnet_push.png", 1.f, 1.f);
	mResourceManager.AddImage(data / "gfx", "magnet_off.png", 1.f, 1.f);
	mResourceManager.AddImage(data / "gfx", "target.png", 200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(data / "gfx", "cursor_pull.png", 32*METERS_PER_PIXEL, 32*METERS_PER_PIXEL);
	mResourceManager.AddImage(data / "gfx", "cursor_push.png", 32*METERS_PER_PIXEL, 32*METERS_PER_PIXEL);
	mResourceManager.AddImage(data / "gfx", "cursor_default.png", 32*METERS_PER_PIXEL, 32*METERS_PER_PIXEL);
	// -- add new images here
	mResourceManager.AddImage(data / "gfx" / "maps", "1_lvl.png", 1408*METERS_PER_PIXEL, 832*METERS_PER_PIXEL);
	mResourceManager.AddImage(data / "gfx", "titlescreen.png", 1024*METERS_PER_PIXEL, 600*METERS_PER_PIXEL);
	mResourceManager.AddImage(data / "gfx", "empty.png", 150*METERS_PER_PIXEL, 10*METERS_PER_PIXEL);

	mResourceManager.AddSoundBuffer(data / "snd", "collide.ogg", "collide");

	sf::Font f;
	f.LoadFromFile((data / "Capture it.ttf").string());
	mResourceManager.AddFont(f, "custom");

	SetSubtext("");

	mMusic.OpenFromFile((data / "THA-defensemechanisms.ogg").string());
	mMusic.SetLoop(true);
	mMusic.Play();

	mRenderWin->ShowMouseCursor(false);
	mRenderWin->SetCursorPosition(WIDTH / 2, HEIGHT / 2);

	// Load The World
	LoadWorld(data);
}

void GameApp::Run() {
	const float fps = 60.f;
	const float dt = 1/fps;
	float time_budget = 0.f;

	while(mRenderWin->IsOpened()) {
		sf::Event event;
		while(mRenderWin->GetEvent(event)) {
			if(event.Type == sf::Event::Closed)
				Quit();
			if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Comma) {
				mDebugGrid = !mDebugGrid;
			}
			mWorld.HandleEvent(event);
		}

		float time_delta = mClock.GetElapsedTime();
		mClock.Reset();

		if(mAppMode == AM_PLAY && mWorld.GetCurrentLevel() != 0)
			mTotalTime += time_delta;

		// SFML access class for real-time input
		const sf::Input& input = mRenderWin->GetInput();
		float frameTime = mRenderWin->GetFrameTime();

		time_budget += time_delta;
		// Update simulation with fixed timestep.
		while(time_budget >= dt) {
			mWorld.Update(frameTime);
			time_budget -= dt;
		}

		mRenderWin->Clear(sf::Color(0,0,0));
		SetGuiPaintingMode(false);

		// Draw World
		mWorld.Draw(mRenderWin.get(), mShader);

		// Draw debug grid
		if (mDebugGrid) {
			Vector2D topleft = Coordinates::ScreenPixelToWorldFloat(Vector2D(0,0));
			Vector2D bottomright = Coordinates::ScreenPixelToWorldFloat(Vector2D(mRenderWin->GetWidth(), mRenderWin->GetHeight()));
			Coordinates tmp;
			tmp.SetWorldPixel(Vector2D(mRenderWin->GetWidth(), mRenderWin->GetHeight()));
			Vector2D size = tmp.GetWorldFloat();
			Coordinates start, end;
			sf::Shape line;
			sf::Text t;
			t.SetCharacterSize(12);
			sf::Color c = sf::Color(255,255,255,100);
			t.SetColor(c);
			// draw vertical line
			for (int x = floor(topleft.x); x < ceil(bottomright.x); ++x) {
				start.SetWorldFloat(Vector2D(x,topleft.y));
				end.SetWorldFloat(Vector2D(x, bottomright.y));
				int w = 1;
				if (x == 0) w = 3;
				line = sf::Shape::Line(start.GetWorldPixel().x, start.GetWorldPixel().y, end.GetWorldPixel().x, end.GetWorldPixel().y, w, c);
				mRenderWin->Draw(line);

				t.SetPosition(start.GetWorldPixel().x, start.GetWorldPixel().y);
				t.SetString(boost::lexical_cast<std::string>(x)+" m");
				mRenderWin->Draw(t);
			}
			// draw horizontal line
			for (int y = floor(topleft.y); y < ceil(bottomright.y); ++y) {
				start.SetWorldFloat(Vector2D(topleft.x,y));
				end.SetWorldFloat(Vector2D(bottomright.x, y));
				int w = 1;
				if (y == 0) w = 3;
				line = sf::Shape::Line(start.GetWorldPixel().x, start.GetWorldPixel().y, end.GetWorldPixel().x, end.GetWorldPixel().y, w, c);
				mRenderWin->Draw(line);

				t.SetPosition(start.GetWorldPixel().x, start.GetWorldPixel().y);
				t.SetString(boost::lexical_cast<std::string>(y)+" m");
				mRenderWin->Draw(t);
			}
		}


		SetGuiPaintingMode(true);
		if(IsEditorMode()) {
			sf::Text fps_text("Layer "+boost::lexical_cast<std::string>(mWorld.GetEditorLayer()) +
							  " / " + boost::lexical_cast<std::string>(round(1/frameTime)) + " FPS");
			fps_text.SetCharacterSize(10);
			fps_text.SetStyle(sf::Text::Regular);
			fps_text.SetPosition(10,10);
			mRenderWin->Draw(fps_text);

			sf::Text ec_text(boost::lexical_cast<std::string>(mWorld.GetEntityCount())); // entitiycount
			ec_text.SetCharacterSize(12);
			ec_text.SetStyle(sf::Text::Regular);
			ec_text.SetPosition(10,30);
			mRenderWin->Draw(ec_text);

			mSubtext.SetCharacterSize(14);
			mSubtext.SetStyle(sf::Text::Bold);
			mSubtext.SetPosition(round(mRenderWin->GetWidth() / 2 - mSubtext.GetRect().Width / 2), mRenderWin->GetHeight() - 20);
			mRenderWin->Draw(mSubtext);
		} else if(mAppMode != AM_EDITOR) {
			// Puzzle information
			if(mAppMode == AM_PUZZLE) {
				sf::Text t("Place a Mover by clicking on the rail.");
				t.SetCharacterSize(18);
				t.SetPosition(floor(WIDTH / 2 - t.GetRect().Width / 2), 80);
				t.SetFont(mResourceManager.GetFont("custom"));
				mRenderWin->Draw(t);
			} else if(mWorld.GetCurrentLevel() != 0){
				boost::posix_time::time_duration td = boost::posix_time::seconds(mTotalTime);
				sf::Text t(boost::lexical_cast<std::string>(td.minutes()) + ":" + leadingZeros(td.seconds(), 2) );
				t.SetCharacterSize(50);
				t.SetPosition(WIDTH - t.GetRect().Width - 20, 20);
				t.SetFont(mResourceManager.GetFont("custom"));
				mRenderWin->Draw(t);
			}

		}
		// Cursor
		if(mAppMode != AM_PLAY)
			mCursor.SetImage(mResourceManager.GetImage("cursor_default"));
		else if(mRenderWin->GetInput().IsMouseButtonDown(sf::Mouse::Left))
			mCursor.SetImage(mResourceManager.GetImage("cursor_push"));
		else
			mCursor.SetImage(mResourceManager.GetImage("cursor_pull"));
		mCursor.SetOrigin(mCursor.GetImage()->GetHeight() / 2, mCursor.GetImage()->GetWidth() / 2);

		mCursor.SetPosition(mRenderWin->GetInput().GetMouseX(),mRenderWin->GetInput().GetMouseY());
		mRenderWin->Draw(mCursor);

		mRenderWin->Display();
	}
}

void GameApp::Quit() {
	mRenderWin->Close();
}

void GameApp::LoadWorld(const boost::filesystem::path& data_path) {
	mWorld.Initialize(data_path);
	mWorld.Load();
}

void GameApp::SetAppMode(AppMode mode) {
	mAppMode = mode;
}

bool GameApp::IsEditorMode() const {
	return mAppMode == AM_EDITOR;
}

AppMode GameApp::GetAppMode() const {
	return mAppMode;
}


sf::View& GameApp::GetView() {
	return *mView.get();
}

void GameApp::SetGuiPaintingMode(bool guipaint) {
	if (guipaint)
		mRenderWin->SetView(mRenderWin->GetDefaultView());
	else
		mRenderWin->SetView(*mView);
}

const sf::Input& GameApp::GetInput() const {
	return mRenderWin.get()->GetInput();
}

Vector2D GameApp::GetMousePosition() const {
	return Vector2D(GetInput().GetMouseX(), GetInput().GetMouseY());
}

const Vector2D GameApp::GetWindowSize() const {
	return Vector2D(mRenderWin->GetWidth(), mRenderWin->GetHeight());
}

int GameApp::GetNextId() {
	return mNextId++;
}

void GameApp::SetNextId(int id) {
	mNextId = id+1;
}

World* const GameApp::GetWorldPtr() {
	return &mWorld;
}

ResourceManager* const GameApp::GetResourceManagerPtr() {
	return &mResourceManager;
}

void GameApp::SetSubtext(const std::string& subtext) {
	mSubtext.SetString(subtext);
}

boost::shared_ptr<sf::RenderWindow> GameApp::GetRenderWindowPtr() {
	return mRenderWin;
}

