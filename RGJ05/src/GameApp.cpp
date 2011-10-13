#include "GameApp.hpp"

GameApp::GameApp() {
	mNextId = 1300;
	mDebugGrid = false;
}

GameApp::~GameApp() {}

void GameApp::Init() {
	mRenderWin = boost::shared_ptr<sf::RenderWindow>(new sf::RenderWindow);
	mRenderWin->Create(sf::VideoMode(WIDTH, HEIGHT, 32), "Love Robot From Space.");
	mRenderWin->EnableVerticalSync(true);

	mView = boost::shared_ptr<sf::View>(new sf::View(sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(WIDTH, HEIGHT)));
	mRenderWin->SetView(*mView.get());

	// load shader
	mRenderWin->SetActive(true);
	// shader is broken ;( disabled
	//mShader.LoadFromFile("../data/SaturationShader.glsl");
	//mShader.SetTexture("tex", sf::Shader::CurrentTexture);


	// load resources
	boost::filesystem::path gfx("../gfx/");
	mResourceManager.AddImage(gfx, "loverobot.svg", 200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "heart.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "box.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "crystal1.svg",	200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "crystal2.svg",	200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "crystal3.svg",	200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "crystal4.svg",	200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "grass2.svg",	200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "grass1.svg",	200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "tree1.svg",	200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "flare.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "lightray1.svg", 200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "lightray2.svg", 100*METERS_PER_PIXEL, 50*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "lightray3.svg", 100*METERS_PER_PIXEL, 50*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "lightray4.svg", 100*METERS_PER_PIXEL, 50*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "lightray5.svg", 100*METERS_PER_PIXEL, 50*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "lightray6.svg", 100*METERS_PER_PIXEL, 50*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "platform.svg",	200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "rock1.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "rock2.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "rock3.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "rock4.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "rock5.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "rock6.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "rock7.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "rock8.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "rock9.svg",		200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	mResourceManager.AddImage(gfx, "spaceship.svg",	200*METERS_PER_PIXEL, 200*METERS_PER_PIXEL);
	// -- add new images here

	mViewBorder.LoadFromFile("../gfx/view_border.png");

	SetSubtext("Hint: <Tab> for the editor!");

	mMusic.OpenFromFile("../data/music.ogg");
	mMusic.SetLoop(true);
	mMusic.Play();

	// Load The World
	LoadWorld();
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
			if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Quote) {
				mDebugGrid = !mDebugGrid;
			}
			mWorld.HandleEvent(event);
		}

		float time_delta = mClock.GetElapsedTime();
		mClock.Reset();

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


		if (!mIsInEditorMode) {

			SetGuiPaintingMode(true);
			// Draw view border
			sf::Sprite vb(mViewBorder);
			mRenderWin->Draw(vb);
			// Draw darkness shape
			sf::Shape s = sf::Shape::Rectangle(0,0, WIDTH, HEIGHT,sf::Color(0,0,0,255*1.f/3.f - 255*mWorldHearts*2/9.f));
			mRenderWin->Draw(s);

			SetGuiPaintingMode(false);
		}

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
		std::string mode = "Game Mode";
		if (mIsInEditorMode) mode = "Editor Mode - Layer "+boost::lexical_cast<std::string>(mWorld.GetEditorLayer());
		sf::Text fps_text(boost::lexical_cast<std::string>(round(1/frameTime)) + " ("+mode+")");
		fps_text.SetCharacterSize(12);
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

		mRenderWin->Display();
	}
}

void GameApp::Quit() {
	mRenderWin->Close();
}

void GameApp::LoadWorld() {
	mWorld.Initialize();

	SetWorldHearts(0);

	mWorld.Load();
}

void GameApp::ToggleEditorMode() {
	if (mIsInEditorMode)
		mWorld.Save();
	mIsInEditorMode = !mIsInEditorMode;
}
bool GameApp::IsEditorMode() {
	return mIsInEditorMode;
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

sf::Color GameApp::GetSpriteColor(const float alpha) const {

	//TODO: We need a shader.
//	float v = mWorldHearts/3.f * 255; // from 0 to 255, depending on mWorldHearts
	float v = 255;
	return sf::Color(v,v,v, alpha * 255);
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

void GameApp::SetWorldHearts(int h) {
	mWorldHearts = h;
	//mShader.SetParameter("new_sat", mWorldHearts / 6.f + 0.5f);
}
int GameApp::GetWorldHearts() const {
	return mWorldHearts;
}

boost::shared_ptr<sf::RenderWindow> GameApp::GetRenderWindowPtr() {
	return mRenderWin;
}
