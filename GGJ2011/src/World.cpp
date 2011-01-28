#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include "World.hpp"
#include "GameApp.hpp"


World::World() {
	mEditorMouseAction = EMA_NONE;
	mEditorPolygonFinished = true;
	mEditorRailFinished = true;
	mEditorLayer = 1;
}

World::~World() {}

void World::Initialize() {
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

	mDynamicsWorld->setGravity(btVector3(0, 10, 0));

	mDebugDraw = boost::shared_ptr<DebugDraw> (new DebugDraw(GameApp::get_mutable_instance().GetRenderWindowPtr()));

	mDynamicsWorld->setDebugDrawer(mDebugDraw.get());

	mDynamicsWorld->setInternalTickCallback(myTickCallback, static_cast<void *>(this));

	mLocalLayerCount = 0;

}

void World::Update(const float time_delta) {
	// INPUT!
	const sf::Input& in = GameApp::get_mutable_instance().GetInput();
	sf::View& view = GameApp::get_mutable_instance().GetView();
	if(GameApp::get_mutable_instance().IsEditorMode()) {
		float px = 10;
		if(in.IsKeyDown(sf::Key::LShift) || in.IsKeyDown(sf::Key::RShift)) {
			px *= 5;
		}
		if(in.IsKeyDown(sf::Key::Up)) {
			view.SetCenter(view.GetCenter().x, view.GetCenter().y - px);
		}
		if(in.IsKeyDown(sf::Key::Down)) {
			view.SetCenter(view.GetCenter().x, view.GetCenter().y + px);
		}
		if(in.IsKeyDown(sf::Key::Left)) {
			view.SetCenter(view.GetCenter().x - px, view.GetCenter().y);
		}
		if(in.IsKeyDown(sf::Key::Right)) {
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
		Rail& r = mRails.back();
		Coordinates tmp;
		tmp.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());
		float d = r.ClosestPositionOnLine(tmp.GetWorldPixel());
		Vector2D p = r.GetPointFromFloat(d);
		mClosestMarker = sf::Shape::Circle(p.x,p.y,5,sf::Color(255,255,255,128));
	}

	//mDynamicsWorld->stepSimulation(time_delta, 10);
	mDynamicsWorld->stepSimulation(1 / 60.f, 10);
	mDynamicsWorld->clearForces();

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
	if(mEntityListNeedsSorting) {
		mEntities.sort(Entity::SortHelper);
		mEntityListNeedsSorting = false;
	}

	mEditorEnabledRenameThisFrame = false;
}

void World::Draw(sf::RenderTarget* target, sf::Shader& shader) {
	auto entity_iter = mEntities.begin();
	// draw first 3 layers
	int layers_before_collision_polygons = 3;
	if (GameApp::get_mutable_instance().IsEditorMode() && mEditorLayer == 0)
		layers_before_collision_polygons = 9;

	for(; entity_iter != mEntities.end() && entity_iter->GetLayer() <= layers_before_collision_polygons; ++entity_iter) {
		entity_iter->Draw(target, shader, GameApp::get_mutable_instance().IsEditorMode() && mEditorLayer == entity_iter->GetLayer());
	}

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

	// draw the rest
	for(; entity_iter != mEntities.end(); ++entity_iter) {
		entity_iter->Draw(target, shader, GameApp::get_mutable_instance().IsEditorMode() && mEditorLayer == entity_iter->GetLayer());
	}

	// draw the rails
	BOOST_FOREACH(Rail& r, mRails) {
		r.Draw(target, sf::Color(128,128,128));
	}
	target->Draw(mClosestMarker);

	if(mEditorMouseAction == EMA_ROTATE && mEditorMouseActionEntity != NULL) {
		Coordinates start, end;
		start.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());
		end.SetWorldFloat(mEditorMouseActionEntity->GetPosition());
		sf::Shape line = sf::Shape::Line(start.GetWorldPixel().x,start.GetWorldPixel().y, end.GetWorldPixel().x, end.GetWorldPixel().y, 1,sf::Color(255,0,255));
		target->Draw(line);
	}

	mDynamicsWorld->debugDrawWorld();
}

void World::AddEntity(Entity* entity) {
	mEntities.push_back(entity);
	mEntityListNeedsSorting = true;
}

void World::HandleEvent(const sf::Event& event) {
	if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Tab) {
		GameApp& app = GameApp::get_mutable_instance();
		if(app.IsEditorMode()) {
			app.SetAppMode(AM_PUZZLE);
		} else {
			app.SetAppMode(AM_EDITOR);
		}
		ToggleSetMouseAction(EMA_NONE); // stop grabbing etc.
	} else if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Escape && !mEditorRenameMode) {
		// menu or quit or so
		if (GameApp::get_mutable_instance().IsEditorMode())
			Save();
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
				if(event.Key.Code == sf::Key::Return || event.Key.Code == sf::Key::Escape) {
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
				if(event.Key.Code == sf::Key::F1) {
					Save();
				} else if(event.Key.Code == sf::Key::F2) {
					ReloadTriMeshBody();
				} else if(event.Key.Code == sf::Key::O) {
					//mDebugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
					if(mDebugDraw->getDebugMode() == btIDebugDraw::DBG_NoDebug)
						mDebugDraw->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
					else if(mDebugDraw->getDebugMode() == btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE)
						mDebugDraw->setDebugMode(btIDebugDraw::DBG_NoDebug);
					else {
						mDebugDraw->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
					}
				} else if(event.Key.Code == sf::Key::S) {
					ToggleSetMouseAction(EMA_SCALE);
				} else if(event.Key.Code == sf::Key::R) {
					ToggleSetMouseAction(EMA_ROTATE);
				} else if(event.Key.Code == sf::Key::G) {
					ToggleSetMouseAction(EMA_GRAB);
				} else if(event.Key.Code == sf::Key::A) {
					ToggleSetMouseAction(EMA_ALPHA);
				} else if(event.Key.Code == sf::Key::Return) {
					ToggleSetMouseAction(EMA_ACCEPT);
				} else if(event.Key.Code == sf::Key::C) {
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
				} else if(event.Key.Code == sf::Key::X) {
					//delete entity
					if (mEditorSelectedEntity!=NULL) {
						DeleteEntityByUID(mEditorSelectedEntity->GetUID());
						SetSelectedEntity(NULL);
					}
				} else if(event.Key.Code == sf::Key::E) {
					//next image
					if (mEditorSelectedEntity!=NULL)
						mEditorSelectedEntity->SetImage(GetImageKeyRelativeTo(mEditorSelectedEntity->GetImage(), +1));
				} else if(event.Key.Code == sf::Key::W) {
					//prev entity
					if (mEditorSelectedEntity!=NULL)
						mEditorSelectedEntity->SetImage(GetImageKeyRelativeTo(mEditorSelectedEntity->GetImage(), -1));
				} else if(event.Key.Code == sf::Key::F) {
					// move up 1 layer
					if(mEditorSelectedEntity!=NULL && mEditorSelectedEntity->GetLayer()<9) {
						Entity* e = mEditorSelectedEntity;
						int n = e->GetLayer()+1;
						e->SetLayer(n);
						mEntityListNeedsSorting = true;
						SetEditorLayer(n);
						SetSelectedEntity(e);
					}
				} else if(event.Key.Code == sf::Key::V) {
					// move down 1 layer and follow
					if(mEditorSelectedEntity!=NULL && mEditorSelectedEntity->GetLayer()>1) {
						Entity* e = mEditorSelectedEntity;
						int n = e->GetLayer()-1;
						e->SetLayer(n);
						mEntityListNeedsSorting = true;
						SetEditorLayer(n);
						SetSelectedEntity(e);
					}
				} else if(event.Key.Code == sf::Key::PageUp) {
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
				} else if(event.Key.Code == sf::Key::PageDown) {
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
				} else if(event.Key.Code == sf::Key::N) {
					if (mEditorSelectedEntity!=NULL) {
						SetRenameMode(true);
					}
				} else if(event.Key.Code == sf::Key::P) {
					if (mEditorSelectedEntity!=NULL) {
						mEditorSelectedEntity->TogglePhysics(*this);
					}
				}

				// -- Layer selection
				else if(event.Key.Code == sf::Key::Num1 || event.Key.Code == sf::Key::Numpad1) {
					SetEditorLayer(1);
				} else if(event.Key.Code == sf::Key::Num2 || event.Key.Code == sf::Key::Numpad2) {
					SetEditorLayer(2);
				} else if(event.Key.Code == sf::Key::Num3 || event.Key.Code == sf::Key::Numpad3) {
					SetEditorLayer(3);
				} else if(event.Key.Code == sf::Key::Num4 || event.Key.Code == sf::Key::Numpad4) {
					SetEditorLayer(4);
				} else if(event.Key.Code == sf::Key::Num5 || event.Key.Code == sf::Key::Numpad5) {
					SetEditorLayer(5);
				} else if(event.Key.Code == sf::Key::Num6 || event.Key.Code == sf::Key::Numpad6) {
					SetEditorLayer(6);
				} else if(event.Key.Code == sf::Key::Num7 || event.Key.Code == sf::Key::Numpad7) {
					SetEditorLayer(7);
				} else if(event.Key.Code == sf::Key::Num8 || event.Key.Code == sf::Key::Numpad8) {
					SetEditorLayer(8);
				} else if(event.Key.Code == sf::Key::Num9 || event.Key.Code == sf::Key::Numpad9) {
					SetEditorLayer(9);
				} else if(event.Key.Code == sf::Key::Num0 || event.Key.Code == sf::Key::Numpad0) {
					SetEditorLayer(0);
				}
				// -- End layer selection
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
					mRails.back().SetNextPoint(tmp.GetWorldPixel());
					mEditorRailFinished = mRails.back().IsFinished();
				} else if(event.MouseButton.Button == sf::Mouse::Right) {
					if (mEditorPolygonFinished) {
						// delete polygon
						Rail* r = GetClosestRail();
						if (r != NULL) {
							for(auto iter = mRails.begin(); iter != mRails.end(); ++iter) {
								if (r->GetCenter() == iter->GetCenter()) {
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
	} else {
		//GAME!!!
		/*
		if (event.Type == sf::Event::KeyPressed) {
			Entity* p = GetEntityByUID("player");
			if (p != NULL) {
				p->SetUsePhysics(*this, true);
				p->GetBody()->setActivationState(DISABLE_DEACTIVATION);
				if(event.Key.Code == sf::Key::Space) {
					bool can_jump = false;
					btTransform xform;
					p->GetBody()->getMotionState()->getWorldTransform(xform);
					btVector3 source = xform.getOrigin();
					btVector3 target = source;
					target[1] = target[1] + 1.f;

					btCollisionWorld::ClosestRayResultCallback result(source, target);
					mDynamicsWorld->rayTest(source, target, result);
					//std::cout << result.hasHit() << std::endl;
					//std::cout << "(source) x: " << source[0] << " y: " << source[1] << " z: " << source[2] << std::endl;
					//std::cout << "(target) x: " << target[0] << " y: " << target[1] << " z: " << target[2] << std::endl;

					btVector3 relativeImpulse = btVector3(0, 5, 0);
					btMatrix3x3& boxRot = p->GetBody()->getWorldTransform().getBasis();
					btVector3 correctedImpulse = boxRot * relativeImpulse;
					p->GetBody()->applyCentralImpulse(correctedImpulse);
				}
			} else {
				std::cerr << "There is no player entity. Name one of them 'player'!" << std::endl;
			}

		}
		*/
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
	std::cout << ":: Saving entities..." << std::endl;
	if(mEntities.size() > 0) {
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
		FILE* file = fopen("../data/levels.info","w");
		fclose(file);
		write_info("../data/levels.info", pt);
	}
}
void World::Load() {
	std::cout << "::Clearing all entities..." << std::endl;
	BOOST_FOREACH(Entity& entity, mEntities) {
		mDynamicsWorld->removeRigidBody(entity.GetBody().get());
	}
	mEntities.clear();
	mCollisionPolygons.clear();
	mRails.clear();

	using boost::property_tree::ptree;
	ptree pt;
	std::cout << ":: Loading entities and collision polygons..." << std::endl;

	if(boost::filesystem::exists("../data/levels.info")) {
		if(!boost::filesystem::is_empty("../data/levels.info")) {
			read_info("../data/levels.info", pt);

			BOOST_FOREACH(ptree::value_type &v, pt.get_child("entities")) {
				mEntities.push_back(new Entity());
				mEntities.back().Initialize(*this, &pt, v.first.data());
			}
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("polygons")) {
				mCollisionPolygons.push_back(new CollisionPolygon());
				mCollisionPolygons.back().Load(&pt, boost::lexical_cast<int>(v.first.data()));
			}
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("rails")) {
				mRails.push_back(new Rail());
				mRails.back().Load(&pt, boost::lexical_cast<int>(v.first.data()));
			}
		}
	} else {
		FILE* file = fopen("../data/levels.info","w");
		fclose(file);
	}
	if(mEntities.size() > 0) {
		std::string lastuid = mEntities.back().GetUID();
		std::vector<std::string> strs;
		boost::split(strs, lastuid, boost::is_any_of("-"));
		GameApp::get_mutable_instance().SetNextId(boost::lexical_cast<int>(strs.back()));
	}
	ReloadTriMeshBody();

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
					Entity* a = (Entity*)obA->getUserPointer();
					Entity* b = (Entity*)obB->getUserPointer();
					// do something!
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

Rail* World::GetClosestRail() {
	Rail* closest = NULL;
	Coordinates tmp;
	tmp.SetWorldPixel(Vector2D(20,0));
	float min_d = tmp.GetWorldFloat().x;

	tmp.SetScreenPixel(GameApp::get_mutable_instance().GetMousePosition());

	BOOST_FOREACH(Rail& r, mRails) {
		float d = (r.GetCenter() - tmp.GetWorldPixel()).Magnitude();
		if (d < min_d) {
			closest = &r;
			min_d = d;
		}
	}
	return closest;
}


void World::AddRigidBody(btRigidBody* body) {
	mDynamicsWorld->addRigidBody(body);
}

void World::RemoveRigidBody(btRigidBody* body) {
	mDynamicsWorld->removeRigidBody(body);
}

void World::ReloadTriMeshBody() {
	if(mCollisionPolygons.size() > 0) {
		if (mTriMeshBody != NULL)
			mDynamicsWorld->removeRigidBody(mTriMeshBody.get());
		mTriMesh = boost::shared_ptr<btTriangleMesh>(new btTriangleMesh(true, false));
		BOOST_FOREACH(CollisionPolygon& p, mCollisionPolygons) {
			std::vector<Vector2D> points = p.GetPoints();
			mTriMesh->addTriangle(-btVector3(points.at(0).x,points[0].y,0),-btVector3(points[1].x,points[1].y,0),-btVector3(points[2].x,points[2].y,0));
		}
		mTriMeshShape = boost::shared_ptr<btBvhTriangleMeshShape>(new btBvhTriangleMeshShape(mTriMesh.get(),true));
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(0,0,0));
		transform.setRotation(btQuaternion(0,0,1,0));
		mTriMeshBodyMotionState = boost::shared_ptr<btDefaultMotionState>(new btDefaultMotionState(transform));
		btRigidBody::btRigidBodyConstructionInfo tm_info(btScalar(0), mTriMeshBodyMotionState.get(), mTriMeshShape.get(), btVector3(0,0,0));

		mTriMeshBody = boost::shared_ptr<btRigidBody>(new btRigidBody(tm_info));

		mDynamicsWorld->addRigidBody(mTriMeshBody.get());
	}
}

Entity* World::GetEntityByLocalLayerId(int ll) {
	BOOST_FOREACH(Entity& entity, mEntities) {
		if(entity.GetLocalLayer() == ll) {
			return &entity;
		}
	}
	return NULL;
}
