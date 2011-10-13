#include "GameApp.hpp"

GameApp::GameApp(const uint32_t width, const uint32_t height, const std::string& name, const float speed) {
	m_width = width;
	m_height = height;
	m_name = name;
	m_speed = speed;
}

GameApp::~GameApp() {}

bool GameApp::Init() {
	m_RenderWin.Create(sf::VideoMode(m_width, m_height, 32), m_name);
	m_RenderWin.ShowMouseCursor(false);
	
	m_Input = &m_RenderWin.GetInput();

	return true;
}

void GameApp::Run() {
	// Physics setup
	b2Vec2 gravity(0.f, 0.f);
	bool do_sleep = true;
	b2World world(gravity, do_sleep);

	// Player ship
	Ship PlayerShip(1.f, "gfx/ship.png");
	PlayerShip.Spawn(320.f, 430.f, &world);

	// Asteroids
	AsteroidManager AsteroidMan(world);

	// Stars
	StarfieldManager StarfieldMan;
	StarfieldMan.SetMaxStars(20);

	// Background
	sf::Image background_img;
	background_img.LoadFromFile("gfx/background.png");
	sf::Sprite background(background_img);

	// Foreground
	sf::Image foreground_img;
	foreground_img.LoadFromFile("gfx/foreground.png");
	foreground_img.SetSmooth(false);
	sf::Sprite foreground1(foreground_img);
	foreground1.SetPosition(0, 0);
	sf::Sprite foreground2(foreground_img);
	foreground1.SetPosition(800, 0);

	// Shield overlay
	sf::Image shield_img;
	shield_img.LoadFromFile("gfx/shield.png");
	shield_img.SetSmooth(false);
	sf::Sprite shield(shield_img);
	shield.SetPosition(0, m_height - shield.GetSize().y);

	// Shield bar
	sf::Image shieldbar_img;
	shieldbar_img.LoadFromFile("gfx/shieldbar.png");
	shieldbar_img.SetSmooth(false);
	sf::Sprite shieldbar(shieldbar_img);
	shieldbar.SetPosition(28, m_height - 42);

	// FPS stuff
	sf::Text fps_text;
	fps_text.SetCharacterSize(16.f);
	fps_text.SetPosition(10.f, 10.f);
	fps_text.SetColor(sf::Color(255, 255, 255, 100));

	// Score stuff
	uint32_t score = 0;
	sf::Text score_text;
	score_text.SetCharacterSize(32.f);
	score_text.SetPosition(m_width / 3.f, 20.f);
	score_text.SetColor(sf::Color(255, 255, 255, 100));

	// Level stuff
	m_level = 0;
	sf::Text level_text;
	level_text.SetCharacterSize(48.f);
	level_text.SetPosition(m_width - 200.f, 20.f);
	level_text.SetColor(sf::Color(255, 255, 255, 100));

	// Physics rate setup
	float dt = 1.f / 60.f;
	float accumulator = 0.f;
	int32 velocity_iterations = 6;
	int32 position_iterations = 2;
	bool drawn = false;

	// Collision
	sf::Image collision_img;
	collision_img.LoadFromFile("gfx/collision.png");
	sf::Sprite collision(collision_img);
	sf::Vector2f size = collision.GetSize();
	collision.SetOrigin(size.x / 2, size.y / 2);
	bool collided = false;

	// Pause and instructions stuff
	sf::Image instruct1_img;
	instruct1_img.LoadFromFile("gfx/instruct1.png");
	sf::Sprite instruct1(instruct1_img);
	instruct1.SetPosition(200, 50);

	sf::Image instruct2_img;
	instruct2_img.LoadFromFile("gfx/instruct2.png");
	sf::Sprite instruct2(instruct2_img);
	instruct2.SetPosition(200, 50);

	sf::Image paused_img;
	paused_img.LoadFromFile("gfx/paused.png");
	sf::Sprite paused(paused_img);
	paused.SetPosition(400, 450);
	m_pause_mode = 1;

	sf::Image gameover_img;
	gameover_img.LoadFromFile("gfx/gameover.png");
	sf::Sprite gameover(gameover_img);
	gameover.SetPosition(200, 50);

	sf::Music Music;
	Music.OpenFromFile("snd/THA-impactevent.ogg");
	Music.Play();

	// Enter main loop.
	while(m_RenderWin.IsOpened()) {
		// Get non-gameplay events like game quit
		GetEvents();

		// Set up timestep and reset clock
		float elapsed = m_Clock.GetElapsedTime();
		accumulator += elapsed;
		m_Clock.Reset();

		// Calculate FPS 
		float framerate = 1.f / elapsed;
		fps_text.SetString("FPS: " + boost::lexical_cast<std::string>(uint16_t(framerate)));

		// Check for unpause
		if(m_pause_mode >= 3)
			m_pause_mode = 0;

		while(accumulator >= dt) {
			if(m_pause_mode == 0) {
				// Physics and gameplay updates
				AsteroidMan.Update(elapsed, world);
				StarfieldMan.Update(elapsed);

				score += 50;
				score_text.SetString("Score: "+boost::lexical_cast<std::string>(score));

				m_level = score / 10000;
				level_text.SetString("Level: "+boost::lexical_cast<std::string>(m_level));

											// Sekrit formula!
				AsteroidMan.SetMaxAsteroids(5 * (m_level * 1.5) + 1 + m_level*2);

				collided = false;
				float collide_health = PlayerShip.GetHealth();

				//b2WorldManifold worldManifold;
				for(const b2ContactEdge* ce = PlayerShip.GetContactList(); ce; ce = ce->next) {
					//b2Contact* c = ce->contact;
					//c->GetWorldManifold(&worldManifold);

					// Take damage
					if(PlayerShip.GetHealth() > 0)
						PlayerShip.SetHealth(PlayerShip.GetHealth() - 30);
					else {
						// Lose, reset and show game over
						m_pause_mode = -1;
						PlayerShip.SetHealth(PlayerShip.GetMaxHealth());
						AsteroidMan.Reset();
						score = 0;
						m_level = 0;
					}
				}

				// Recharge shield
				float health = PlayerShip.GetHealth();
				float max_health = PlayerShip.GetMaxHealth();
				if(health < collide_health) {
					collided = true;
					//collision.SetPosition(worldManifold.points->x, 
					//					  worldManifold.points->y);
					sf::Vector2f playership_size = PlayerShip.GetDrawable().GetSize();
					sf::Vector2f playership_pos = PlayerShip.GetDrawable().GetPosition();
					collision.SetPosition(playership_pos.x - (playership_size.x / 2), 
										  playership_pos.y + (playership_size.y / 2));
				}
				if(health < max_health) {
					PlayerShip.SetHealth(health + 0.5f);
				}
				shieldbar.SetScaleX(health / max_health);

				// Slide foreground fog
				if(foreground1.GetPosition().x <= -800)
					foreground1.SetPosition(800, 0);
				if(foreground2.GetPosition().x <= -800)
					foreground2.SetPosition(800, 0);
				foreground1.Move(-10.f, 0);
				foreground2.Move(-10.f, 0);

				world.Step(dt, velocity_iterations, position_iterations);
				world.ClearForces();

				PlayerShip.Update(m_Input);
			}

			accumulator -= dt;
			drawn = false;
		}

		if(drawn) {
			sf::Sleep(0.01);
		}
		else {
			// Draw everything
			m_RenderWin.Clear(sf::Color(10, 10, 10));
			m_RenderWin.Draw(background);

			boost::ptr_vector<Star> stars;
			stars = StarfieldMan.GetStarVector();
			BOOST_FOREACH(Star& Star, stars) {
				m_RenderWin.Draw(Star.GetDrawable());
			}

			boost::ptr_vector<Asteroid> asteroids;
			asteroids = AsteroidMan.GetAsteroidVector();
			BOOST_FOREACH(Asteroid& Asteroid, asteroids) {
				m_RenderWin.Draw(Asteroid.GetDrawable());
				//m_RenderWin.Draw(Asteroid.GetDebugDrawable());
			}

			m_RenderWin.Draw(PlayerShip.GetDrawable());
			//m_RenderWin.Draw(PlayerShip.GetDebugDrawable());

			m_RenderWin.Draw(shield);
			m_RenderWin.Draw(shieldbar);

			if(collided)
				m_RenderWin.Draw(collision);

			if(m_pause_mode > 0)
				m_RenderWin.Draw(paused);
			if(m_pause_mode == 1)
				m_RenderWin.Draw(instruct1);
			if(m_pause_mode == 2)
				m_RenderWin.Draw(instruct2);
			if(m_pause_mode == -1)
				m_RenderWin.Draw(gameover);

			m_RenderWin.Draw(fps_text);
			m_RenderWin.Draw(score_text);
			m_RenderWin.Draw(level_text);

			m_RenderWin.Draw(foreground1);
			m_RenderWin.Draw(foreground2);

			m_RenderWin.Display();

			drawn = true;
		}
	}
}

void GameApp::GetEvents() {
	// Process events
	sf::Event Event;
	while(m_RenderWin.GetEvent(Event)) {
		// Window closed
		if(Event.Type == sf::Event::Closed)
			m_RenderWin.Close();
		// Escape key pressed
		if((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
			m_RenderWin.Close();
		// Pause switcher
		if((Event.Type == sf::Event::KeyPressed) && (m_pause_mode > 0))
			++m_pause_mode;
		if((Event.Type == sf::Event::KeyPressed) && (m_pause_mode == -1))
			m_pause_mode = 2;
	}
}
