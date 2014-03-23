#include "enemy.hpp"

void Enemy::init(SDL_Texture* tex, btVector3 pos){
    position = pos; 
    texture = tex;
}

void Enemy::update(float dt) {

}
void Enemy::event(SDL_Event& event) {

}

