#include "map.hpp"
#include <SDL2_gfxPrimitives.h>

void Map::onInit(){
    physicsShape = createMap();    
}

btBvhTriangleMeshShape* Map::createMap(){
    btTriangleMesh* triMesh = new btTriangleMesh();

    std::vector<btVector3> triangle1 = {btVector3(10,20,0),btVector3(20, 20, 0), btVector3(30, 20, 0)};
    std::vector<btVector3> triangle2 = {btVector3(100, 300, 0),btVector3(130, 340, 0), btVector3(150, 300, 0)};
    std::vector<btVector3> triangle3 = {btVector3(200, 400, 0),btVector3(250, 400, 0), btVector3(250, 400, 0)};
    std::vector<btVector3> triangle4 = {btVector3(400, 200, 0),btVector3(300, 230, 0), btVector3(320, 220, 0)};
    
    triMesh->addTriangle(triangle1[0],triangle1[1],triangle1[2]);
    triMesh->addTriangle(triangle2[0],triangle2[1],triangle2[2]);
    triMesh->addTriangle(triangle3[0],triangle3[1],triangle3[2]);
    triMesh->addTriangle(triangle4[0],triangle4[1],triangle4[2]);

    m_Vector.push_back(triangle1);
    m_Vector.push_back(triangle2);
    m_Vector.push_back(triangle3);
    m_Vector.push_back(triangle4);

       btBvhTriangleMeshShape* triMeshShape = new btBvhTriangleMeshShape(triMesh, true, true); 

    return triMeshShape;
}

void Map::onDraw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
    for (std::size_t count = 0; count < m_Vector.size(); count ++){
        SDL_Point points[4];
        points[0] = {int (m_Vector[count][0].x()), int (m_Vector[count][0].y())};
        points[1] = {int (m_Vector[count][1].x()), int (m_Vector[count][1].y())};
        points[2] = {int (m_Vector[count][2].x()), int (m_Vector[count][2].y())};
        points[3] = {int (m_Vector[count][0].x()), int (m_Vector[count][0].y())};
        SDL_RenderDrawLines(renderer, points, 4);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

