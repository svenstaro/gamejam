#ifndef DEBUGDRAW_HPP
#define DEBUGDRAW_HPP

#include <SDL.h>
#include <SDL_pixels.h>
#include <bullet/btBulletDynamicsCommon.h>

class DebugDraw : public btIDebugDraw
{
public:
    DebugDraw(SDL_Renderer* renderer);

    /**
     * @brief Converts a Bullet color (float 0.0f - 1.0f range) to an SDL color (Uint8 0 - 255 range)
     * @param color A Bullet color object
     * @return An SDL_color struct
     */
    static SDL_Color btToSDLColor(const btVector3& color);

	void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
	//void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3 &toColor);
    //void drawSphere(btScalar radius, const btTransform &transform, const btVector3 &color);
    //void drawSphere(const btVector3 &p, btScalar radius, const btVector3 &color);
    //void drawTriangle(const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar);
    void drawContactPoint(const btVector3& point_on_b, const btVector3& normal_on_b, btScalar distance, int life_time, const btVector3& color);
    void reportErrorWarning(const char* warning_string);
    void draw3dText(const btVector3& location, const char* text_string);

    void setDebugMode(int mode);
	int getDebugMode() const;

    //void drawAabb(const btVector3 &from, const btVector3 &to, const btVector3 &color);

private:
    int m_debugMode;
    SDL_Renderer* m_Renderer;
};

#endif
