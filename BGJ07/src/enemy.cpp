#include "enemy.hpp"

void Enemy::init(SDL_Texture* tex, btVector3 pos){
    position = pos; 
    texture = tex;
}

const std::string Enemy::getId() const {
    return "Enemy";
}

void Enemy::update(float dt) {

}
void Enemy::event(SDL_Event& event) {

}

