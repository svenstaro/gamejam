#include "debugDraw.hpp"

#include <iostream>

DebugDraw::DebugDraw(SDL_Renderer* renderer) {
    m_Renderer = renderer;
}

SDL_Color DebugDraw::btToSDLColor(const btVector3& color) {
	SDL_Color result = {Uint8(color.x() * 255), Uint8(color.y() * 255), Uint8(color.z() * 255)};
	return result;
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    auto c = btToSDLColor(color);
    SDL_SetRenderDrawColor(m_Renderer, c.r, c.g, c.b, 255);
    SDL_RenderDrawLine(m_Renderer, from.x(), from.y(), to.x(), to.y());
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
}

/*
void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& from_color, const btVector3& to_color) {
    auto c = btToSDLColor(from_color);
    aalineRGBA(m_Renderer, from.x(), from.y(), to.x(), to.y(), c.r, c.g, c.b, 255);
}

void DebugDraw::drawSphere(btScalar radius, const btTransform& transform, const btVector3& color) {
    auto c = btToSDLColor(color);
    aacircleRGBA(m_Renderer, transform.getOrigin().x(), transform.getOrigin().y(), radius, c.r, c.g, c.b, 255);
}

void DebugDraw::drawSphere(const btVector3& p, btScalar radius, const btVector3& color) {
    auto c = btToSDLColor(color);
    aacircleRGBA(m_Renderer, p.x(), p.y(), radius, c.r, c.g, c.b, 255);
}

void DebugDraw::drawTriangle(const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar) {
    auto c = btToSDLColor(color);
    aatrigonRGBA(m_Renderer, v0.x(), v0.y(), v1.x(), v1.y(), v2.x(), v2.y(), c.r, c.g, c.b, 255);
}
*/

void DebugDraw::drawContactPoint(const btVector3& point_on_b, const btVector3& normal_on_b, btScalar distance, int life_time, const btVector3& color) {
    drawLine(point_on_b, point_on_b + normal_on_b * distance, color);
}

void DebugDraw::reportErrorWarning(const char* warning_string) {
    std::cerr << warning_string << std::endl;
}

void DebugDraw::draw3dText(const btVector3& location, const char* text_string) {
}

void DebugDraw::setDebugMode(int mode) {
    m_debugMode = mode;
}

int DebugDraw::getDebugMode() const {
    return m_debugMode;
}

//void DebugDraw::drawAabb(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    //auto c = btToSDLColor(color);
    //boxRGBA(m_Renderer, from.x(), from.y(), to.x(), to.y(), c.r, c.g, c.b, 255);
//}
