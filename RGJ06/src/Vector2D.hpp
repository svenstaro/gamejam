#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#define PI 3.14159265358979323846

#include <cmath>
#include <iostream>
#include <SFML/System.hpp>

class Vector2D
{

public:
	Vector2D(float x = 0, float y = 0);
	~Vector2D() {}

	void Rotate( const float angle );
	float Magnitude() const;
	float Rotation() const;
	float Normalize();
	float DotProduct( const Vector2D& v2 ) const;
	float CrossProduct( const Vector2D& v2 ) const;

	static Vector2D Zero();
	static float Distance( const Vector2D& v1, const Vector2D& v2);
	static float Angle( const Vector2D& v1, const Vector2D& v2);
	static float rad2Deg( const float rad );
	static float deg2Rad( const float deg );
	
	Vector2D& operator= ( const Vector2D& v2 );

	Vector2D& operator+= ( const Vector2D& v2 );
	Vector2D& operator-= ( const Vector2D& v2 );
	Vector2D& operator*= ( const float scalar);
	Vector2D& operator/= ( const float scalar);

	const Vector2D operator+( const Vector2D &v2 ) const;
	const Vector2D operator-( const Vector2D &v2 ) const;
	const Vector2D operator*( const float scalar ) const;
	const Vector2D operator/( const float scalar ) const;

	bool operator== ( const Vector2D& v2 ) const;
	bool operator!= ( const Vector2D& v2 ) const;

    const sf::Vector2f GetSfmlVector() const;
public:
	float x, y;
};

std::ostream& operator<< (std::ostream& stream, Vector2D vector);

#endif
