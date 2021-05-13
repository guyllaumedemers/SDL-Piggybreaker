#pragma once

#include <SDL.h>

class Color {
public:
	Uint8 r, g, b, a;

	Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};

class Vector;

class Point {
private:
	//	drag and drop data
	static const int NEAR_DISTANCE = 40;
	bool isMoveable = false;
 	bool isDragMode = false;

	bool isNear(int x, int y);

public:
	//	attributs
	double x, y;

	//	constructeurs
	Point();
	Point(double x, double y, bool isMoveable = false);

	bool isDragged();

	//	méthodes spécifiques
	double sqrDist(const Point& p) const;
	void draw(SDL_Renderer *renderer, Color c, int taille) const;
	void drawCircle(SDL_Renderer *renderer, int rayon, Color c, bool continu) const;

	void update(SDL_Event &evt);

	Point operator+(const Vector & v) const;
	Point operator-(const Vector & v) const;
	Point & operator+=(const Vector & v);
	Point & operator-=(const Vector & v);
};

class Vector : public Point {
public:
	//	-	constructors
	Vector();
	Vector(double angle);
	Vector(double x, double y);
	Vector(Point p1, Point p2);

	//	-	methodes
	void normalize();
	double magnitude();

	//	-	operators
	//	scalar product and division
	Vector operator*(const double k) const;
	Vector operator/(const double k) const;
	Vector & operator*=(const double k);
	Vector & operator/=(const double k);
	Vector operator-();

	//	addition and subtraction of 2 vectors
	Vector operator+(const Vector & v2) const;
	Vector operator-(const Vector & v2) const;
	Vector & operator+=(const Vector & v2);
	Vector & operator-=(const Vector & v2);
	
	//	dot product and cross product
	double operator*(const Vector & v2) const;
	double operator^(const Vector & v2) const;
};

//	scalar product in regular order
Vector operator*(double k, const Vector & v);
