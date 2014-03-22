#include "player.hpp"
#include <SDL_keyboard.h>
#include <SDL_keycode.h>

void Player::init(SDL_Texture* tex, btVector3 pos){
    position = pos; 
    texture = tex;
}

void Player::update(float dt) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
/*    if (keystate[SDLK_LEFT])
        physicsBody.applyLinearForce(btVector3(-1*dt, 0, 0));
    else if (keystate[SDLK_RIGHT])
        physicsBody.applyLinearForce(btVector3(1*dt, 0, 0));
    else if (keystate[SDLK_UP])
        physicsBody.applyLinearForce(btVector3(0, 1*dt, 0));
    else if (keystate[SDLK_DOWN])
        physicsBody.applyLinearForce(btVector3(0, -1*dt, 0));
*/
}

void Player::event(SDL_Event& event) {

}

