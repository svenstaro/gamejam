#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include "World.hpp"
#include "GameApp.hpp"

std::string leadingZeros2(int i, int n) {
   std::string s = boost::lexical_cast<std::string>(i);
   while(s.length() < n) {
	   s = "0" + s;
   }
   return s;
}

World::World() {
	mEditorMouseAction = EMA_NONE;
	mEditorPolygonFinished = true;
	mEditorRailFinished = true;
	mClosestRailPoint = NULL;
	mEditorLayer = 1;
	mCurrentLevel = 0;
	mNumLevels = 10;
	mSelectedMoverType = 1;
	mDrawDebugs = false;
}

World::~World() {}

void World::Initialize(const boost::filesystem::path& data_path) {
    mDataPath = data_path;

	mCollisionConfiguration = boost::shared_ptr<btDefaultCollisionConfiguration>(new btDefaultCollisionConfiguration());
	mCollisionDispatcher = boost::shared_ptr<btCollisionDispatcher>(new btCollisionDispatcher(mCollisionConfiguration.get()));
	mBroadphase = boost::shared_ptr<btDbvtBroadphase>(new btDbvtBroadphase());
	mSimplex = boost::shared_ptr<btVoronoiSimplexSolver>(new btVoronoiSimplexSolver());
	mPdSolver = boost::shared_ptr<btMinkowskiPenetrationDepthSolver>(new btMinkowskiPenetrationDepthSolver());
	mSolver = boost::shared_ptr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver());
	mDynamicsWorld = boost::shared_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(mCollisionDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfiguration.get()));

	mConvexAlgo2d = boost::shared_ptr<btConvex2dConvex2dAlgorithm::CreateFunc>(new btConvex2dConvex2dAlgorithm::CreateFunc(mSimplex.get(), mPdSolver.get()));

	mCollisionDispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE, mConvexAlgo2d.get());
	mCollisionDispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE, mConvexAlgo2d.get());
	mCollisionDispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, mConvexAlgo2d.get());
	//mCollisionDispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, new btBox2dBox2dCollisionAlgorithm::CreateFunc());
	mCollisionDispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, mBox2dAlgo2d.get());

	mDynamicsWorld->setGravity(btVector3(0, 3, 0));

	mDebugDraw = boost::shared_ptr<DebugDraw> (new DebugDraw(GameApp::get_mutable_instance().GetRenderWindowPtr()));

	mDebugDraw->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);

	mDynamicsWorld->setDebugDrawer(mDebugDraw.get());

	mDynamicsWorld->setInternalTickCallback(myTickCallback, static_cast<void *>(this));

	mLocalLayerCount = 0;

}

void World::Update(const float time_delta) {
	mCurrentLevelTime += time_delta;
	sf::View& view = GameApp::get_mutable_instance().GetView();
	if(GameApp::get_mutable_instance().IsEditorMode()) {
		float px = 10;
		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::IsKeyPressed(sf::Keyboard::RShift)) {
			px *= 5;
		}
		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::Up)) {
			view.SetCenter(view.GetCenter().x, view.GetCenter().y - px);
		}
		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::Down)) {
			view.SetCenter(view.GetCenter().x, view.GetCenter().y + px);
		}
		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::Left)) {
			view.SetCenter(view.GetCenter().x - px, view.GetCenter().y);
		}
		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::Right)) {
			view.SetCenter(view.GetCenter().x + px, view.GetCenter().y);
		}

		Coordinates mp;
		mp.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());
		if(mEditorMouseAction == EMA_GRAB) {
			mEditorMouseActionEntity->SetPosition( mEditorMouseActionStartEntityPosition.GetWorldFloat() +
												   mp.GetWorldFloat() -
												   mEditorMouseActionStartMousePosition.GetWorldFloat() );
		} else if(mEditorMouseAction == EMA_ALPHA) {
			float mpf = mp.GetScreenFloat().y;
			float mspf = mEditorMouseActionStartMousePosition.GetScreenFloat().y;
			float d = (mspf - mpf) * 5.f;
			float a = mEditorMouseActionStartEntityAlpha + d;
			if (a > 1) a = 1;
			if (a < 0) a = 0;
			mEditorMouseActionEntity->SetAlpha( a );
		} else if(mEditorMouseAction == EMA_SCALE) {
			Vector2D md = mEditorMouseActionStartEntityPosition.GetScreenPixel() - mp.GetScreenPixel();
			Vector2D sd = mEditorMouseActionStartEntityPosition.GetScreenPixel() - mEditorMouseActionStartMousePosition.GetScreenPixel();
			float f =  md.Magnitude() / sd.Magnitude();
			mEditorMouseActionEntity->SetScale( mEditorMouseActionStartEntityScale * f );
		} else if(mEditorMouseAction == EMA_ROTATE) {
			Coordinates ep;
			ep.SetWorldFloat(mEditorMouseActionEntity->GetPosition());
			mEditorMouseActionEntity->SetRotation(
					(mEditorMouseActionStartMousePosition.GetWorldFloat()-ep.GetWorldFloat()).Rotation()
					-(mp.GetWorldFloat()-ep.GetWorldFloat()).Rotation()
					+ mEditorMouseActionStartEntityRotation );
		}
	} else if(GameApp::get_mutable_instance().GetAppMode() == AM_PUZZLE) {
		// draw point on closest rail
		Rail* r = GetClosestRail();
		if(r != NULL) {
			Coordinates tmp;
			tmp.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());
			float d = r->ClosestPositionOnLine(tmp.GetWorldFloat());
			mClosestRailPoint = r->GetPointFromFloat(d);
		}
		mClosestRail = r;

	}
	if(GetBoxEntity() != NULL && GetBoxEntity()->UsesPhysics() && !sf::Mouse::IsButtonPressed(sf::Mouse::Left)) {
		mCurrentRail = GetClosestRail(true, GetBoxEntity()->GetBody()->getWorldTransform().getOrigin());
	}

	//mDynamicsWorld->stepSimulation(time_delta, 10);
	if(GameApp::get_mutable_instance().GetAppMode() == AM_PLAY) {
		mDynamicsWorld->stepSimulation(1 / 60.f, 10);
		mDynamicsWorld->clearForces();
	}

	Entity* c = GetClosestEntityOnLayer(GameApp::get_mutable_instance().GetMousePosition(), mEditorLayer);
	BOOST_FOREACH(Entity& entity, mEntities) {
		int h = 0;
		if (mEditorSelectedEntity == &entity) h = 2;
		else if (c == &entity) h = 1;
		entity.SetHighlightMode(h);
		entity.Update(time_delta);
		/*if(entity.GetLifeTime() >= entity.GetTimeToLive()) {
			mDynamicsWorld->removeRigidBody(entity.GetBody().get());
			mEntities.erase_if(boost::bind(&Entity::GetUID, _1) == entity.GetEntityUniqueId());
		}*/
	}
	// draw the rails
	BOOST_FOREACH(Rail& r, mRails) {
		r.Update(time_delta);
	}
	if(mEntityListNeedsSorting) {
		mEntities.sort(Entity::SortHelper);
		mEntityListNeedsSorting = false;
	}

	mEditorEnabledRenameThisFrame = false;

	if(mCurrentLevelTime > mCurrentLevelMessage * 5 && mCurrentLevelMessage < mLevelMessages.size()) {
		GameApp::get_mutable_instance().Msg(mLevelMessages[mCurrentLevelMessage]);
		mCurrentLevelMessage++;
	}
}

void World::Draw(sf::RenderTarget* target, sf::Shader& shader) {
	GameApp::get_mutable_instance().SetGuiPaintingMode(false);
	// draw background
	if(mCurrentLevel > 0) {
		sf::Sprite back(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("level"+leadingZeros2(mCurrentLevel, 2)));
		target->Draw(back);
	}

	auto entity_iter = mEntities.begin();
	// draw first 3 layers
	int layers_before_collision_polygons = 3;
	if (GameApp::get_mutable_instance().IsEditorMode() && mEditorLayer == 0)
		layers_before_collision_polygons = 9;

	for(; entity_iter != mEntities.end() && entity_iter->GetLayer() <= layers_before_collision_polygons; ++entity_iter) {
		entity_iter->Draw(target, shader, GameApp::get_mutable_instance().IsEditorMode() && mEditorLayer == entity_iter->GetLayer());
	}

	if(GameApp::get_mutable_instance().IsEditorMode()) {
		CollisionPolygon* cl = GetClosestCollisionPolygon();
		int i = 0;
		BOOST_FOREACH(CollisionPolygon& p, mCollisionPolygons) {
			sf::Color b;
			if (GameApp::get_mutable_instance().IsEditorMode() && mEditorLayer==0) {
				if (i == mCollisionPolygons.size()-1 && !mEditorPolygonFinished)
					b = sf::Color::Magenta;
				else if (cl == &p)
					b = sf::Color(255,129,0);
				else
					b = sf::Color::White;
			} else b = sf::Color(0,0,0,0);
			p.Draw(target, b);
			++i;
		}
	}

	// draw the rest
	for(; entity_iter != mEntities.end(); ++entity_iter) {
		entity_iter->Draw(target, shader, GameApp::get_mutable_instance().IsEditorMode() && mEditorLayer == entity_iter->GetLayer());
	}

	// draw the rails
	Rail* cr = GetClosestRail();
	BOOST_FOREACH(Rail& r, mRails) {
		r.Draw(target, shader, cr == &r && mEditorLayer == 9);
	}

	if(mClosestRail != NULL) {
		Vector2D p = Coordinates::WorldFloatToWorldPixel(mClosestRailPoint);
		target->Draw(sf::Shape::Circle(p.x, p.y, 5, sf::Color(255,255,255,128)));
	}

	if(mEditorMouseAction == EMA_ROTATE && mEditorMouseActionEntity != NULL) {
		Coordinates start, end;
		start.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());
		end.SetWorldFloat(mEditorMouseActionEntity->GetPosition());
		sf::Shape line = sf::Shape::Line(start.GetWorldPixel().x,start.GetWorldPixel().y, end.GetWorldPixel().x, end.GetWorldPixel().y, 1,sf::Color(255,0,255));
		target->Draw(line);
	}

	if(mDrawDebugs)
		mDynamicsWorld->debugDrawWorld();

	if(GameApp::get_mutable_instance().GetAppMode() == AM_PUZZLE) {
		//GUI
		Vector2D b1(target->GetWidth() / 2 - 50, 42);
		Vector2D b2(target->GetWidth() / 2, 42);
		Vector2D b3(target->GetWidth() / 2 + 50, 42);

		sf::Sprite s1(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("magnet_off"));
		sf::Sprite s2(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("spring_off"));
		sf::Sprite s3(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("static_off"));

		sf::Sprite sel(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("selector"));
		sel.SetOrigin(330 / 2, 0);
		sel.SetScale(0.5,0.6);
		sel.SetPosition(target->GetWidth() / 2, 10);
		sel.SetColor(sf::Color(255,255,255,180));

		s1.SetOrigin(32,32);
		s2.SetOrigin(32,32);
		s3.SetOrigin(32,32);

		s1.SetScale(0.6, 0.6);
		s2.SetScale(0.6, 0.6);
		s3.SetScale(0.6, 0.6);

		s1.SetPosition(b1.x, b1.y);
		s2.SetPosition(b2.x, b2.y);
		s3.SetPosition(b3.x, b3.y);

		sf::Text t1(boost::lexical_cast<std::string>(MoversOfTypeLeft(MT_MAGNET)));
		sf::Text t2(boost::lexical_cast<std::string>(MoversOfTypeLeft(MT_SPRING)));
		sf::Text t3(boost::lexical_cast<std::string>(MoversOfTypeLeft(MT_STATIC)));

		t1.SetCharacterSize(16);
		t2.SetCharacterSize(16);
		t3.SetCharacterSize(16);

		t1.SetColor(sf::Color(150,150,150));
		t2.SetColor(sf::Color(150,150,150));
		t3.SetColor(sf::Color(150,150,150));

		if(mSelectedMoverType == 1) t1.SetColor(sf::Color::White);
		if(mSelectedMoverType == 2) t2.SetColor(sf::Color::White);
		if(mSelectedMoverType == 3) t3.SetColor(sf::Color::White);

		t1.SetFont(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetFont("custom"));
		t2.SetFont(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetFont("custom"));
		t3.SetFont(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetFont("custom"));

		t1.SetPosition(b1.x - t1.GetRect().Width / 2, b1.y + 31);
		t2.SetPosition(b2.x - t2.GetRect().Width / 2, b2.y + 31);
		t3.SetPosition(b3.x - t3.GetRect().Width / 2, b3.y + 31);

		GameApp::get_mutable_instance().SetGuiPaintingMode(true);

		target->Draw(sel);

		target->Draw(s1);
		target->Draw(s2);
		target->Draw(s3);

		target->Draw(t1);
		target->Draw(t2);
		target->Draw(t3);
	}
}

void World::AddEntity(Entity* entity) {
	mEntities.push_back(entity);
	mEntityListNeedsSorting = true;
}

void World::HandleEvent(const sf::Event& event) {
	GameApp::get_mutable_instance().SetGuiPaintingMode(false);
	if(event.Type == sf::Event::MouseButtonPressed && event.MouseButton.Button == sf::Mouse::Left && !GameApp::get_mutable_instance().IsEditorMode()) {
		Vector2D mp = GameApp::get_mutable_instance().GetMousePosition();

        auto target = GameApp::get_mutable_instance().GetRenderWindowPtr();
		Vector2D b1(target->GetWidth() - 1 * 70 - 10,42);
		Vector2D b2(target->GetWidth() - 2 * 70 - 10,42);

		if((b2-mp).Magnitude() < 32) {
			// click on b2
			if (GameApp::get_mutable_instance().GetAppMode() == AM_PUZZLE) {
				// go
				GameApp::get_mutable_instance().SetAppMode(AM_PLAY);
			} else {
				// build
				GameApp::get_mutable_instance().SetAppMode(AM_PUZZLE);
				ResetLevel();
			}
			return;
		} else if((b1-mp).Magnitude() < 32) {
			// clicked b2
			// retry
			ResetLevel();
			return;
		}
	}

	if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Tab) {
		GameApp& app = GameApp::get_mutable_instance();
		if(app.IsEditorMode()) {
			app.SetAppMode(AM_PUZZLE);
		} else {
			app.SetAppMode(AM_EDITOR);
		}
		ToggleSetMouseAction(EMA_NONE); // stop grabbing etc.
	} else if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::M) {
		GameApp& app = GameApp::get_mutable_instance();
		app.ToggleMute();
	} else if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Escape && !mEditorRenameMode) {
		// menu or quit or so
		// no more autosave
		/*if (GameApp::get_mutable_instance().IsEditorMode())
			Save();*/
		GameApp::get_mutable_instance().Quit();
	} else if(GameApp::get_mutable_instance().IsEditorMode()) {
		// EDITOR
		std::vector<std::string> img_keys = GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImageKeys();
		if (img_keys.size() == 0) {
			std::cerr << "There are no images. The Editor cannot be used when there are no images loaded. Exiting." << std::endl;
			exit(1);
		}

		Vector2D mp = GameApp::get_mutable_instance().GetMousePosition();

		if (event.Type == sf::Event::KeyPressed) {
			if(mEditorRenameMode) {
				if(event.Key.Code == sf::Keyboard::Return || event.Key.Code == sf::Keyboard::Escape) {
					Entity* e = GetEntityByUID(mEditorRenameString);
					if (e!=NULL) {
						SetRenameMode(false);
						GameApp::get_mutable_instance().SetSubtext("An entity with that name already exists.");
					}
					else {
						mEditorSelectedEntity->SetUID(mEditorRenameString);
						SetRenameMode(false);
					}
				}
			} else {
				if(event.Key.Code == sf::Keyboard::F1) {
					Save();
				} else if(event.Key.Code == sf::Keyboard::F2) {
					ReloadTriMeshBody();
				} else if(event.Key.Code == sf::Keyboard::O) {
					mDrawDebugs = !mDrawDebugs;
				} else if(event.Key.Code == sf::Keyboard::S) {
					ToggleSetMouseAction(EMA_SCALE);
				} else if(event.Key.Code == sf::Keyboard::R) {
					ToggleSetMouseAction(EMA_ROTATE);
				} else if(event.Key.Code == sf::Keyboard::G) {
					ToggleSetMouseAction(EMA_GRAB);
				} else if(event.Key.Code == sf::Keyboard::A) {
					ToggleSetMouseAction(EMA_ALPHA);
				} else if(event.Key.Code == sf::Keyboard::Return) {
					ToggleSetMouseAction(EMA_ACCEPT);
				} else if(event.Key.Code == sf::Keyboard::C) {
					//create entity
					Entity* e = new Entity();
					if (mEditorSelectedEntity != NULL) {
						// copy properties
						e->Initialize(*this, mEditorSelectedEntity->GetImage(),mEditorLayer);
						e->SetLocalLayer(mLocalLayerCount);
						mLocalLayerCount++;
						e->SetRotation(mEditorSelectedEntity->GetRotation());
						e->SetScale(mEditorSelectedEntity->GetScale());
						e->SetAlpha(mEditorSelectedEntity->GetAlpha());
					} else {
						e->Initialize(*this, img_keys[0],mEditorLayer);
					}
					e->SetPosition(Coordinates::ScreenPixelToWorldFloat(GameApp::get_mutable_instance().GetMousePosition()));
					AddEntity(e);
					SetSelectedEntity(&mEntities.back());
				} else if(event.Key.Code == sf::Keyboard::X) {
					//delete entity
					if (mEditorSelectedEntity!=NULL) {
						DeleteEntityByUID(mEditorSelectedEntity->GetUID());
						SetSelectedEntity(NULL);
					}
				} else if(event.Key.Code == sf::Keyboard::E) {
					//next image
					if (mEditorSelectedEntity!=NULL)
						mEditorSelectedEntity->SetImage(GetImageKeyRelativeTo(mEditorSelectedEntity->GetImage(), +1));
				} else if(event.Key.Code == sf::Keyboard::W) {
					//prev entity
					if (mEditorSelectedEntity!=NULL)
						mEditorSelectedEntity->SetImage(GetImageKeyRelativeTo(mEditorSelectedEntity->GetImage(), -1));
				} else if(event.Key.Code == sf::Keyboard::F) {
					// move up 1 layer
					if(mEditorSelectedEntity!=NULL && mEditorSelectedEntity->GetLayer()<9) {
						Entity* e = mEditorSelectedEntity;
						int n = e->GetLayer()+1;
						e->SetLayer(n);
						mEntityListNeedsSorting = true;
						SetEditorLayer(n);
						SetSelectedEntity(e);
					}
				} else if(event.Key.Code == sf::Keyboard::V) {
					// move down 1 layer and follow
					if(mEditorSelectedEntity!=NULL && mEditorSelectedEntity->GetLayer()>1) {
						Entity* e = mEditorSelectedEntity;
						int n = e->GetLayer()-1;
						e->SetLayer(n);
						mEntityListNeedsSorting = true;
						SetEditorLayer(n);
						SetSelectedEntity(e);
					}
				} else if(event.Key.Code == sf::Keyboard::PageUp) {
					// move up 1 layer
					if(mEditorSelectedEntity!=NULL && mEditorSelectedEntity->GetLocalLayer()<9) {
						Entity* e = mEditorSelectedEntity;
						int n = e->GetLocalLayer()+1;
						Entity* e2 = GetEntityByLocalLayerId(n);
						if(e2 != NULL) {
							e2->SetLocalLayer(n-1);
						} else {
							--n;
						}
						e->SetLocalLayer(n);
						mEntityListNeedsSorting = true;
						SetSelectedEntity(e);
					}
				} else if(event.Key.Code == sf::Keyboard::PageDown) {
					// move down 1 layer and follow
					if(mEditorSelectedEntity!=NULL && mEditorSelectedEntity->GetLocalLayer()>1) {
						Entity* e = mEditorSelectedEntity;
						int n = e->GetLocalLayer()-1;
						Entity* e2 = GetEntityByLocalLayerId(n);
						if(e2 != NULL) {
							e2->SetLocalLayer(n+1);
						} else {
							++n;
						}
						e->SetLocalLayer(n);
						mEntityListNeedsSorting = true;
						SetSelectedEntity(e);
					}
				} else if(event.Key.Code == sf::Keyboard::N) {
					if (mEditorSelectedEntity!=NULL) {
						SetRenameMode(true);
					}
				} else if(event.Key.Code == sf::Keyboard::P) {
					if (mEditorSelectedEntity!=NULL) {
						mEditorSelectedEntity->TogglePhysics(*this);
					}
				}

				// -- Layer selection
				else if(event.Key.Code == sf::Keyboard::Num1 || event.Key.Code == sf::Keyboard::Numpad1) {
					SetEditorLayer(1);
				} else if(event.Key.Code == sf::Keyboard::Num2 || event.Key.Code == sf::Keyboard::Numpad2) {
					SetEditorLayer(2);
				} else if(event.Key.Code == sf::Keyboard::Num3 || event.Key.Code == sf::Keyboard::Numpad3) {
					SetEditorLayer(3);
				} else if(event.Key.Code == sf::Keyboard::Num4 || event.Key.Code == sf::Keyboard::Numpad4) {
					SetEditorLayer(4);
				} else if(event.Key.Code == sf::Keyboard::Num5 || event.Key.Code == sf::Keyboard::Numpad5) {
					SetEditorLayer(5);
				} else if(event.Key.Code == sf::Keyboard::Num6 || event.Key.Code == sf::Keyboard::Numpad6) {
					SetEditorLayer(6);
				} else if(event.Key.Code == sf::Keyboard::Num7 || event.Key.Code == sf::Keyboard::Numpad7) {
					SetEditorLayer(7);
				} else if(event.Key.Code == sf::Keyboard::Num8 || event.Key.Code == sf::Keyboard::Numpad8) {
					SetEditorLayer(8);
				} else if(event.Key.Code == sf::Keyboard::Num9 || event.Key.Code == sf::Keyboard::Numpad9) {
					SetEditorLayer(9);
				} else if(event.Key.Code == sf::Keyboard::Num0 || event.Key.Code == sf::Keyboard::Numpad0) {
					SetEditorLayer(0);
				}
				// -- End layer selection

				if(event.Key.Code == sf::Keyboard::Space && mEditorLayer == 9) {
					// toggle initial mover mounted
					Rail* r = GetClosestRail();
					if(r != NULL) {
						r->ToggleForcedInitialState();
						r->SetInitialState(false);
						r->Reinitialize(*this);
					}
				} else if(event.Key.Code == sf::Keyboard::T && mEditorLayer == 9) {
					// toggle initial mover mounted
					Rail* r = GetClosestRail();
					if(r != NULL) {
						r->GetMover().ToggleMoverType();
						r->Reinitialize(*this);
					}
				}
			}

		} else if (event.Type == sf::Event::MouseButtonPressed) {
			if (mEditorLayer == 0) {
				if(event.MouseButton.Button == sf::Mouse::Left) {
					// ADD POINT
					if (mEditorPolygonFinished) {
						// create new polygon
						mCollisionPolygons.push_back(new CollisionPolygon());
					}
					Coordinates tmp;
					tmp.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());
					mCollisionPolygons.back().AddPoint(tmp.GetWorldFloat());
					mEditorPolygonFinished = mCollisionPolygons.back().GetNumPoints() >= 3;
				} else if (event.MouseButton.Button == sf::Mouse::Right) {
					if (mEditorPolygonFinished) {
						// delete polygon
						CollisionPolygon* c = GetClosestCollisionPolygon();
						if (c!=NULL) {
							for(auto iter = mCollisionPolygons.begin(); iter != mCollisionPolygons.end(); ++iter) {
								if (c->GetCenter() == iter->GetCenter()) {
									mCollisionPolygons.erase(iter);
									break;
								}
							}
						}
					}
					// Cancel polygon
					mEditorPolygonFinished = true;
				}
			} else if (mEditorLayer == 9) {
				if(event.MouseButton.Button == sf::Mouse::Left) {
					// SetNextPoint
					if(mEditorRailFinished) {
						// create new Rail
						mRails.push_back(new Rail());
					}
					Coordinates tmp;
					tmp.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());
					mRails.back().SetNextPoint(tmp.GetWorldFloat());
					mEditorRailFinished = mRails.back().IsFinished();
					if(mEditorRailFinished) {
						mRails.back().Initialize(*this);
					}
				} else if(event.MouseButton.Button == sf::Mouse::Right) {
					if (mEditorRailFinished) {
						// delete polygon
						Rail* r = GetClosestRail();
						if (r != NULL) {
							for(auto iter = mRails.begin(); iter != mRails.end(); ++iter) {
								if (r->GetCenter() == iter->GetCenter()) {
									if(r->GetRigidBody() != NULL) {
										RemoveRigidBody(r->GetRigidBody());
									}
									if(r->GetConstraint() != NULL) {
										mDynamicsWorld->removeConstraint(r->GetConstraint());
									}
									mRails.erase(iter);
									break;
								}
							}
						}
					} else if(mRails.size() > 0) {
						auto iter = mRails.end();
						--iter;
						mRails.erase(iter);
						// TODO: Make it work ;)
					}
					// Cancel polygon
					mEditorRailFinished = true;
				}
			} else {
				SetRenameMode(false);
				if(event.MouseButton.Button == sf::Mouse::Left) {
					ToggleSetMouseAction(EMA_ACCEPT);
				} else if(event.MouseButton.Button == sf::Mouse::Right) {
					if (mEditorMouseAction!=EMA_NONE)
						ToggleSetMouseAction(EMA_NONE); // cancel action
					else
						SetSelectedEntity(GetClosestEntityOnLayer(GameApp::get_mutable_instance().GetMousePosition(), mEditorLayer));
				}
			}
		} else if (event.Type == sf::Event::MouseWheelMoved) {
			if (event.MouseWheel.Delta > 0)
				SetEditorLayer(mEditorLayer+1);
			else if (mEditorLayer-1 > 0)
				SetEditorLayer(mEditorLayer-1);
		} else if (event.Type == sf::Event::TextEntered && mEditorRenameMode && !mEditorEnabledRenameThisFrame) {
			if (event.Text.Unicode == 8) {
				//backspace
				if (mEditorRenameString != "") {
					mEditorRenameString = mEditorRenameString.substr(0,mEditorRenameString.length()-1);
				}
			}
			else if((event.Text.Unicode >= 0x0041 && event.Text.Unicode <= 0x005a)||	// A..Z
					(event.Text.Unicode >= 0x0061 && event.Text.Unicode <= 0x007a)||	// a..z
					(event.Text.Unicode >= 0x0030 && event.Text.Unicode <= 0x0039)||	// 0..9
					event.Text.Unicode == 0x002D ||										// -
					event.Text.Unicode == 0x005F) {										// _
				mEditorRenameString += (char)event.Text.Unicode;
			}
			SetSelectedEntity(mEditorSelectedEntity);

		}
	} else if(GameApp::get_mutable_instance().GetAppMode() == AM_PUZZLE){
		// PUZZLE
		if(event.Type == sf::Event::KeyPressed) {
			if(event.Key.Code == sf::Keyboard::Return) {
				GameApp::get_mutable_instance().SetAppMode(AM_PLAY);
			}

			if(event.Key.Code == sf::Keyboard::Num1) {
				mSelectedMoverType = 1;
			}
			if(event.Key.Code == sf::Keyboard::Num2) {
				mSelectedMoverType = 2;
			}
			if(event.Key.Code == sf::Keyboard::Num3) {
				mSelectedMoverType = 3;
			}
		}

		if(event.Type == sf::Event::MouseButtonPressed) {
			if(event.MouseButton.Button == sf::Mouse::Left) {
				if(mClosestRail != NULL && mClosestRail->CanBeChanged()) {
					mClosestRail->SetStartPoint(mClosestRailPoint);
					if(MoversOfTypeLeft((MoverType)mSelectedMoverType) > 0) {
						mClosestRail->SetInitialState(true);
						mClosestRail->GetMover().SetMoverType((MoverType)mSelectedMoverType);
					}
					mClosestRail->Reinitialize(*this);
				}
			} else if(event.MouseButton.Button == sf::Mouse::Right) {
				if(mClosestRail != NULL && mClosestRail->CanBeChanged()) {
					mClosestRail->SetInitialState(false);
					mClosestRail->Reinitialize(*this);
				}
			}
		}
	}

	if(GameApp::get_mutable_instance().GetAppMode() != AM_EDITOR) {
		if(event.Type == sf::Event::KeyPressed) {
			if(event.Key.Code == sf::Keyboard::Up) {
				//LoadNextLevel();
			} if(event.Key.Code == sf::Keyboard::Down) {
				//LoadNextLevel(mCurrentLevel - 1);
			}
		}
	}
}

int World::GetEntityCount() const {
	return sf::Uint16(mEntities.size());
}

std::vector<Entity*> World::GetEntitiesByLayer(int layer) {
	std::vector<Entity*> entities;
	BOOST_FOREACH(Entity& entity, mEntities) {
		if(entity.GetLayer() == layer) {
			entities.push_back(&entity);
		}
	}
	return entities;
}

Entity* World::GetClosestEntityOnLayer(const Vector2D mouse_pos, int layer) {
	Entity* closest = NULL;
	Coordinates tmp;
	tmp.SetWorldPixel(Vector2D(20,0));
	float min_d = tmp.GetWorldFloat().x;

	tmp.SetScreenPixel(mouse_pos);

	BOOST_FOREACH(Entity& entity, mEntities) {
		float d = (entity.GetPosition()-tmp.GetWorldFloat()).Magnitude();
		if(d < min_d) {
			if(entity.GetLayer() == layer || layer == 0) {
				min_d = d;
				closest = &entity;
			}
		}
	}

	return closest;
}

Entity* World::GetEntityByUID(const std::string& uid) {
   	BOOST_FOREACH(Entity& entity, mEntities) {
		if(entity.GetUID() == uid) {
            return &entity;
        }
	} 
    return NULL;
}

void World::DeleteEntityByUID(const std::string& uid) {
	BOOST_FOREACH(Entity& entity, mEntities) {
		if(entity.GetUID() == uid && entity.UsesPhysics()) {
			RemoveRigidBody(entity.GetBody().get());
		}
	}
	mEntities.erase_if(boost::bind(&Entity::GetUID, _1) == uid);
	mEntityListNeedsSorting = true;
}


void World::ToggleSetMouseAction(EditorMouseAction action) {
	if (action == EMA_NONE) {
		//RESET!
		if (mEditorMouseAction == EMA_GRAB) {
			mEditorMouseActionEntity->SetPosition(mEditorMouseActionStartEntityPosition.GetWorldFloat());
		} else if (mEditorMouseAction == EMA_ALPHA) {
			mEditorMouseActionEntity->SetAlpha(mEditorMouseActionStartEntityAlpha);
		} else if (mEditorMouseAction == EMA_ROTATE) {
			mEditorMouseActionEntity->SetRotation(mEditorMouseActionStartEntityRotation);
		} else if (mEditorMouseAction == EMA_SCALE) {
			mEditorMouseActionEntity->SetScale(mEditorMouseActionStartEntityScale);
		}
		mEditorMouseAction = EMA_NONE;
	} else if (action == EMA_ACCEPT) {
		mEditorMouseAction = EMA_NONE;
	} else {
		Coordinates tmp;
		tmp.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());
		Entity* e = mEditorSelectedEntity; //GetClosestEntityOnLayer(GameApp::get_mutable_instance().GetMousePosition(),mEditorLayer);
		if(action == mEditorMouseAction) {
			ToggleSetMouseAction(EMA_NONE);
		} else if(e != NULL) {
			mEditorMouseAction = action;
			mEditorMouseActionStartMousePosition = tmp;

			mEditorMouseActionEntity = e;
			mEditorMouseActionStartEntityPosition.SetWorldFloat(e->GetPosition());
			mEditorMouseActionStartEntityRotation = e->GetRotation();
			mEditorMouseActionStartEntityScale = e->GetScale();
			mEditorMouseActionStartEntityAlpha = e->GetAlpha();
		}
	}
}


std::string World::GetImageKeyRelativeTo(const std::string& original, int offset) {
	const std::vector<std::string> img_keys = GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImageKeys();

	int i = 0;
	int n = img_keys.size();
	for(; i < n; ++i) {
		if (original == img_keys[i]) {
			break;
		}
	}
	i+=offset;
	while (i<0) {
		i+=n;
	}
	while (i>=n) {
		i-=n;
	}

	return img_keys[i];
}


int World::GetEditorLayer() const {
	return mEditorLayer;
}

void World::SetEditorLayer(int layer) {
	if (layer > 9) layer = 9;
	if (layer < 0) layer = 1;
	mEditorLayer = layer;
	SetSelectedEntity(NULL);
	ToggleSetMouseAction(EMA_NONE);

	if (layer == 0)
		mEditorPolygonFinished = true;
}

void World::Save() {
	using boost::property_tree::ptree;
	ptree pt;
	std::cout << ":: Saving level" << std::endl;

	BOOST_FOREACH(Entity& entity, mEntities) {
		entity.Save(&pt);
	}
	int i = 0;
	BOOST_FOREACH(CollisionPolygon& colpol, mCollisionPolygons) {
		colpol.Save(&pt, i);
		++i;
	}
	i = 0;
	BOOST_FOREACH(Rail& rail, mRails) {
		rail.Save(&pt, i);
		++i;
	}
	i = 0;
	BOOST_FOREACH(std::string& s, mLevelMessages) {
		pt.put("messages."+boost::lexical_cast<std::string>(i), s);
		++i;
	}
	for(auto iter = mMoversAvailable.begin(); iter != mMoversAvailable.end(); ++iter) {
		pt.put("movers_available."+boost::lexical_cast<std::string>((int)iter->first), iter->second);
	}
	FILE* file = fopen(&GetCurrentLevelFile()[0],"w");
	fclose(file);
	write_info(GetCurrentLevelFile(), pt);

}
void World::Load() {
	BOOST_FOREACH(Entity& entity, mEntities) {
		if(entity.UsesPhysics()) {
			mDynamicsWorld->removeRigidBody(entity.GetBody().get());
		}
	}
	BOOST_FOREACH(Rail& r, mRails) {
		r.Deinitialize(*this);
	}

	mEntities.clear();
	mCollisionPolygons.clear();
	mRails.clear();
	mLevelMessages.clear();
	mMoversAvailable.clear();

	using boost::property_tree::ptree;
	ptree pt;

	if(boost::filesystem::exists(GetCurrentLevelFile())) {
		if(!boost::filesystem::is_empty(GetCurrentLevelFile())) {
			read_info(GetCurrentLevelFile(), pt);

			pt.put("entities", "");
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("entities")) {
				mEntities.push_back(new Entity());
				mEntities.back().Initialize(*this, &pt, v.first.data());
			}
			pt.put("polygons", "");
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("polygons")) {
				mCollisionPolygons.push_back(new CollisionPolygon());
				mCollisionPolygons.back().Load(&pt, boost::lexical_cast<int>(v.first.data()));
			}
			pt.put("rails", "");
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("rails")) {
				mRails.push_back(new Rail());
				mRails.back().Load(&pt, boost::lexical_cast<int>(v.first.data()));
				mRails.back().Initialize(*this);
			}
			pt.put("messages", "");
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("messages")) {
				mLevelMessages.push_back(v.second.data());
			}
			mCurrentLevelMessage = 0;
			mCurrentLevelTime = 0;
			pt.put("movers_available", "");
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("movers_available")) {
				MoverType m = (MoverType)(boost::lexical_cast<int>(v.first.data()));
				mMoversAvailable[m] = boost::lexical_cast<int>(v.second.data());
			}
		}
	} else {
		FILE* file = fopen(&GetCurrentLevelFile()[0],"w");
		fclose(file);
	}
	ReloadTriMeshBody();
}

void World::ResetLevel() {
	BOOST_FOREACH(Rail& r, mRails) {
		r.Reinitialize(*this);
	}
	BOOST_FOREACH(Entity& e, mEntities) {
		e.Reset(*this);
	}
}

void World::SetRenameMode(bool mode) {
	mEditorRenameMode = mode;
	if (mode) {
		mEditorRenameString = mEditorSelectedEntity->GetUID();
		mEditorEnabledRenameThisFrame = true;
	} else {

	}
	SetSelectedEntity(mEditorSelectedEntity);
}


void World::SetSelectedEntity(Entity* e) {
	mEditorSelectedEntity = e;
	if (e!=NULL) {
		if (mEditorRenameMode)
			GameApp::get_mutable_instance().SetSubtext("Type new name: " + mEditorRenameString + "_");
		else
			GameApp::get_mutable_instance().SetSubtext("Selected Entity: " + e->GetUID());
	} else {
		GameApp::get_mutable_instance().SetSubtext("No entity selected.");
	}
}

void World::TickCallback(btScalar timestep) {
	int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
	for(int i=0;i<numManifolds;i++) {
		btPersistentManifold* contactManifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f) {
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;

				if (obA->getUserPointer()!=NULL && obB->getUserPointer()!=NULL) {
					GameObject* a = (GameObject*)obA->getUserPointer();
					GameObject* b = (GameObject*)obB->getUserPointer();
					// do something!
					if(!a->OnCollide(b))
						return;
					if(!b->OnCollide(a))
						return;
				}
			}
		}
	}
}

void myTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    World* w = static_cast<World *>(world->getWorldUserInfo());
    w->TickCallback(timeStep);
}

CollisionPolygon* World::GetClosestCollisionPolygon() {
	CollisionPolygon* closest = NULL;
	Coordinates tmp;
	tmp.SetWorldPixel(Vector2D(20,0));
	float min_d = tmp.GetWorldFloat().x;

	tmp.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());

	BOOST_FOREACH(CollisionPolygon& p, mCollisionPolygons) {
		float d = (p.GetCenter() - tmp.GetWorldFloat()).Magnitude();
		if (d < min_d) {
			closest = &p;
			min_d = d;
		}
	}
	return closest;
}

Rail* World::GetClosestRail(bool all, btVector3 pos) {
	Rail* closest = NULL;
	Coordinates tmp;
	tmp.SetWorldPixel(Vector2D(20,0));
	float min_d = tmp.GetWorldFloat().x;
	if(all) min_d = 1000000000000;

	if(all) {
		tmp.SetWorldFloat(Vector2D(pos.x(),pos.y()));
	}
	else
		tmp.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());


	BOOST_FOREACH(Rail& r, mRails) {
		if(all && abs(r.GetAngleOfBox(GetBoxEntity())) > Vector2D::deg2Rad(110))
			continue;

		Vector2D pol = r.GetPointFromFloat(r.ClosestPositionOnLine(tmp.GetWorldFloat()));
		float d = (pol - tmp.GetWorldFloat()).Magnitude();
		if (d < min_d) {
			closest = &r;
			min_d = d;
		}
	}
	return closest;
}


void World::AddRigidBody(btRigidBody* body) {
	assert(false);
	//mDynamicsWorld->addRigidBody(body);
}

btDiscreteDynamicsWorld* World::GetDynamicsWorld() {
	return mDynamicsWorld.get();
}

void World::RemoveRigidBody(btRigidBody* body) {
	mDynamicsWorld->removeRigidBody(body);
}

void World::ReloadTriMeshBody() {

	if (mTriMeshBody != NULL)
		mDynamicsWorld->removeRigidBody(mTriMeshBody.get());
	mTriMesh = boost::shared_ptr<btTriangleMesh>(new btTriangleMesh(true, false));
	BOOST_FOREACH(CollisionPolygon& p, mCollisionPolygons) {
		std::vector<Vector2D> points = p.GetPoints();
		mTriMesh->addTriangle(-btVector3(points.at(0).x,points[0].y,0),-btVector3(points[1].x,points[1].y,0),-btVector3(points[2].x,points[2].y,0));
	}
	if(mCollisionPolygons.size() <= 0) {
		mTriMesh->addTriangle(btVector3(0,0,0),btVector3(0,0.01,0),btVector3(0,0,0.01));
	}
	mTriMeshShape = boost::shared_ptr<btBvhTriangleMeshShape>(new btBvhTriangleMeshShape(mTriMesh.get(),true));
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0,0,0));
	transform.setRotation(btQuaternion(0,0,1,0));
	mTriMeshBodyMotionState = boost::shared_ptr<btDefaultMotionState>(new btDefaultMotionState(transform));
	btRigidBody::btRigidBodyConstructionInfo tm_info(btScalar(0), mTriMeshBodyMotionState.get(), mTriMeshShape.get(), btVector3(0,0,0));

	mTriMeshBody = boost::shared_ptr<btRigidBody>(new btRigidBody(tm_info));

	mDynamicsWorld->addRigidBody(mTriMeshBody.get(), COL_WALL, COL_BOX);
}

Entity* World::GetEntityByLocalLayerId(int ll) {
	BOOST_FOREACH(Entity& entity, mEntities) {
		if(entity.GetLocalLayer() == ll) {
			return &entity;
		}
	}
	return NULL;
}


Entity* World::GetBoxEntity() {
	Entity* e = GetEntityByUID("box");
	if(e == NULL)
		std::cout << "No box entity specified. Name one entity 'box'." << std::endl;
	return e;
}

Rail* World::GetCurrentRail() {
	return mCurrentRail;
}

void World::SetCurrentRail(Rail* rail) {
	mCurrentRail = rail;
}

const std::string World::GetCurrentLevelFile() {
	return mDataPath.string()+"level-"+boost::lexical_cast<std::string>(mCurrentLevel)+".info";
}

int World::GetCurrentLevel() const {
	return mCurrentLevel;
}

void World::LoadNextLevel(int level) {
	if(level == -1)
		mCurrentLevel += 1;
	else
		mCurrentLevel = level;

	if(mCurrentLevel > mNumLevels || mCurrentLevel < 0) {
		mCurrentLevel = 0;
	}

	Load();
	if(mCurrentLevel == 0)
		GameApp::get_mutable_instance().SetAppMode(AM_PLAY);
	else
		GameApp::get_mutable_instance().SetAppMode(AM_PUZZLE);

}

int World::MoversOfTypeLeft(MoverType type) {
	int left = mMoversAvailable[type];
	BOOST_FOREACH(Rail& r, mRails) {
		if(r.GetMover().GetMoverType() == type && r.CanBeChanged() && r.IsMounted()) {
			--left;
		}
	}
	return left;
}
