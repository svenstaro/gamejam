#ifndef MAP_HPP
#define MAP_HPP

#include <btBulletDynamicsCommon.h>
#include <vector>
#include <SDL.h>
#include "entity.hpp"

class Map : public Entity{
    public:
        btBvhTriangleMeshShape* createMap();
        void onDraw(SDL_Renderer* renderer) override;
        void onInit() override;
        std::string getId();

    private:
        std::vector<std::vector<btVector3>> m_Vector;
};

#endif 

