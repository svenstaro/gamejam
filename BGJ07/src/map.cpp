#include "map.hpp"
#include <SDL2_gfxPrimitives.h>

const std::string Map::getId() const {
    return "Map";
}

void Map::onInit(){
    physicsShape = createMap();    
}

btBvhTriangleMeshShape* Map::createMap(){
    btTriangleMesh* triMesh = new btTriangleMesh();


    // Walls
    std::vector<btVector3> triangle1 = {btVector3(-10, 0,0),btVector3(1610, 0, 0), btVector3(800, -10, 0)};
    std::vector<btVector3> triangle2 = {btVector3(-10, 1200,0),btVector3(1610, 1200, 0), btVector3(800, 1210, 0)};
    std::vector<btVector3> triangle3 = {btVector3(0, -10, 0),btVector3(0, 1210, 0), btVector3(-10, 600, 0)};
    std::vector<btVector3> triangle4 = {btVector3(1600, -10, 0),btVector3(1600, 1210, 0), btVector3(1610, 600, 0)};

    std::vector<btVector3> triangle5 = {btVector3(150, 0, 0),btVector3(0, 150, 0), btVector3(-10, -10, 0)};
    std::vector<btVector3> triangle6 = {btVector3(0, 80, 0),btVector3(500, -10, 0), btVector3(-10, -10, 0)};
    std::vector<btVector3> triangle7 = {btVector3(300, 0, 0),btVector3(800, 110, 0), btVector3(850, -10, 0)};
    std::vector<btVector3> triangle8 = {btVector3(700, -10, 0),btVector3(850, 250, 0), btVector3(1400, -10, 0)};
    std::vector<btVector3> triangle9 = {btVector3(1100, -10, 0),btVector3(1300, 200, 0), btVector3(1550, -10, 0)};
    std::vector<btVector3> triangle10 = {btVector3(1400, -10, 0),btVector3(1610, 600, 0), btVector3(1600, 0, 0)};
    std::vector<btVector3> triangle11 = {btVector3(1610, 800, 0),btVector3(1400, 1210, 0), btVector3(1610, 1210, 0)};
    std::vector<btVector3> triangle12 = {btVector3(1610, 950, 0),btVector3(1000, 1210, 0), btVector3(1610, 1210, 0)};
    std::vector<btVector3> triangle13 = {btVector3(1000, 1000, 0),btVector3(750, 1210, 0), btVector3(1200, 1210, 0)};
    std::vector<btVector3> triangle14 = {btVector3(750, 1100, 0),btVector3(750, 1210, 0), btVector3(450, 1210, 0)};
    std::vector<btVector3> triangle15 = {btVector3(-10, 1100, 0),btVector3(200, 1210, 0), btVector3(-10, 1200, 0)};
    std::vector<btVector3> triangle16 = {btVector3(-10, 600, 0),btVector3(350, 900, 0), btVector3(-10, 800, 0)};
    std::vector<btVector3> triangle17 = {btVector3(100, 800, 0),btVector3(-10, 500, 0), btVector3(-10, 1210, 0)};
    std::vector<btVector3> triangle18 = {btVector3(350, 350, 0),btVector3(450, 600, 0), btVector3(600, 480, 0)};
    std::vector<btVector3> triangle19 = {btVector3(450, 550, 0),btVector3(900, 550, 0), btVector3(850, 480, 0)};
    std::vector<btVector3> triangle20 = {btVector3(800, 500, 0),btVector3(1200, 500, 0), btVector3(1250, 700, 0)};
    std::vector<btVector3> triangle21 = {btVector3(800, 1000, 0),btVector3(1100, 800, 0), btVector3(750, 730, 0)};

    m_Vector.push_back(triangle1);
    m_Vector.push_back(triangle2);
    m_Vector.push_back(triangle3);
    m_Vector.push_back(triangle4);
    m_Vector.push_back(triangle5);
    m_Vector.push_back(triangle6);
    m_Vector.push_back(triangle7);
    m_Vector.push_back(triangle8);
    m_Vector.push_back(triangle9);
    m_Vector.push_back(triangle10);
    m_Vector.push_back(triangle11);
    m_Vector.push_back(triangle12);
    m_Vector.push_back(triangle13);
    m_Vector.push_back(triangle14);
    m_Vector.push_back(triangle15);
    m_Vector.push_back(triangle16);
    m_Vector.push_back(triangle17);
    m_Vector.push_back(triangle18);
    m_Vector.push_back(triangle19);
    m_Vector.push_back(triangle20);
    m_Vector.push_back(triangle21);

    for (auto vec : m_Vector){
        std::vector<btVector3> backvec = vec;
        for (auto& btvec : backvec){
            btvec.setZ(10);
        }
        triMesh->addTriangle(vec[0],vec[1],vec[2]);
        triMesh->addTriangle(backvec[0],backvec[1],backvec[2]);

        triMesh->addTriangle(backvec[2],backvec[1],vec[1]);
        triMesh->addTriangle(vec[2],backvec[2],vec[1]);

        triMesh->addTriangle(vec[1],backvec[1],backvec[0]);
        triMesh->addTriangle(vec[0],backvec[0],vec[1]);
        
        triMesh->addTriangle(vec[2],backvec[2],backvec[0]);
        triMesh->addTriangle(vec[2],backvec[0],vec[0]);
    }

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

