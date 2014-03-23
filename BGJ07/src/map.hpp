#ifndef MAP_HPP
#define MAP_HPP

#include <btBulletDynamicsCommon.h>
#include <vector>
#include <SDL.h>

class Map{
    public:
        btBvhTriangleMeshShape* createMap();
        void drawLevel(SDL_Renderer* renderer);

    private:
        std::vector<std::vector<btVector3>> m_Vector;
};

#endif 

