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
    std::vector<btVector3> triangle1 = {btVector3(-10, 0,0),btVector3(1610, 0, 0), btVector3(800, -10, 66)};
    std::vector<btVector3> triangle2 = {btVector3(-10, 1200,0),btVector3(1610, 1200, 0), btVector3(800, 1210, 66)};
    std::vector<btVector3> triangle3 = {btVector3(0, -10, 0),btVector3(0, 1210, 0), btVector3(-10, 600, 66)};
    std::vector<btVector3> triangle4 = {btVector3(1600, -10, 0),btVector3(1600, 1210, 0), btVector3(1610, 600, 66)};

    std::vector<btVector3> triangle5 = {btVector3(150, 0, 0),btVector3(0, 150, 0), btVector3(-10, -10, 66)};
    std::vector<btVector3> triangle6 = {btVector3(0, 80, 0),btVector3(500, -10, 0), btVector3(-10, -10, 66)};
    std::vector<btVector3> triangle7 = {btVector3(300, 0, 0),btVector3(800, 110, 0), btVector3(850, -10, 66)};
    std::vector<btVector3> triangle8 = {btVector3(700, -10, 0),btVector3(850, 250, 0), btVector3(1400, -10, 66)};
    std::vector<btVector3> triangle9 = {btVector3(1100, -10, 0),btVector3(1300, 200, 0), btVector3(1550, -10, 66)};
    std::vector<btVector3> triangle10 = {btVector3(1400, -10, 0),btVector3(1610, 600, 0), btVector3(1600, 0, 66)};
    std::vector<btVector3> triangle11 = {btVector3(1610, 800, 0),btVector3(1400, 1210, 0), btVector3(1610, 1210, 66)};
    std::vector<btVector3> triangle12 = {btVector3(1610, 950, 0),btVector3(1000, 1210, 0), btVector3(1610, 1210, 66)};
    std::vector<btVector3> triangle13 = {btVector3(1000, 1000, 0),btVector3(750, 1210, 0), btVector3(1200, 1210, 66)};
    std::vector<btVector3> triangle14 = {btVector3(750, 1100, 0),btVector3(750, 1210, 0), btVector3(450, 1210, 66)};
    std::vector<btVector3> triangle15 = {btVector3(-10, 1100, 0),btVector3(200, 1210, 0), btVector3(-10, 1200, 66)};
    std::vector<btVector3> triangle16 = {btVector3(-10, 600, 0),btVector3(350, 900, 0), btVector3(-10, 800, 66)};
    std::vector<btVector3> triangle17 = {btVector3(100, 800, 0),btVector3(-10, 500, 0), btVector3(-10, 1210, 66)};
    std::vector<btVector3> triangle18 = {btVector3(350, 350, 0),btVector3(450, 600, 0), btVector3(600, 480, 66)};
    std::vector<btVector3> triangle19 = {btVector3(450, 550, 0),btVector3(900, 550, 0), btVector3(850, 480, 66)};
    std::vector<btVector3> triangle20 = {btVector3(800, 500, 0),btVector3(1200, 500, 0), btVector3(1250, 700, 66)};
    std::vector<btVector3> triangle21 = {btVector3(800, 1000, 0),btVector3(1100, 800, 0), btVector3(750, 730, 66)};
    
    triMesh->addTriangle(triangle1[0],triangle1[1],triangle1[2]);
    triMesh->addTriangle(triangle2[0],triangle2[1],triangle2[2]);
    triMesh->addTriangle(triangle3[0],triangle3[1],triangle3[2]);
    triMesh->addTriangle(triangle4[0],triangle4[1],triangle4[2]);
    triMesh->addTriangle(triangle5[0],triangle5[1],triangle5[2]);
    triMesh->addTriangle(triangle6[0],triangle6[1],triangle6[2]);
    triMesh->addTriangle(triangle7[0],triangle7[1],triangle7[2]);
    triMesh->addTriangle(triangle8[0],triangle8[1],triangle8[2]);
    triMesh->addTriangle(triangle9[0],triangle9[1],triangle9[2]);
    triMesh->addTriangle(triangle10[0],triangle10[1],triangle10[2]);
    triMesh->addTriangle(triangle11[0],triangle11[1],triangle11[2]);
    triMesh->addTriangle(triangle12[0],triangle12[1],triangle12[2]);
    triMesh->addTriangle(triangle13[0],triangle13[1],triangle13[2]);
    triMesh->addTriangle(triangle14[0],triangle14[1],triangle14[2]);
    triMesh->addTriangle(triangle15[0],triangle15[1],triangle15[2]);
    triMesh->addTriangle(triangle16[0],triangle16[1],triangle16[2]);
    triMesh->addTriangle(triangle17[0],triangle17[1],triangle17[2]);
    triMesh->addTriangle(triangle18[0],triangle18[1],triangle18[2]);
    triMesh->addTriangle(triangle19[0],triangle19[1],triangle19[2]);
    triMesh->addTriangle(triangle20[0],triangle20[1],triangle20[2]);
    triMesh->addTriangle(triangle21[0],triangle21[1],triangle21[2]);

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

