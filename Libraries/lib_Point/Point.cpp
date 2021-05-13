#include "Point.h"
#include <utility>

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

}

//	***************
//	class of points
//	***************

//	constructors
Point::Point() :Point(50, 50, true) {
}

Point::Point(double x, double y, bool isMoveable) {
	this->x = x;
	this->y = y;
	this->isMoveable = isMoveable;
}

bool Point::isNear(int x, int y) {
	return sqrDist(Point((double)x, (double)y)) <= (long long)NEAR_DISTANCE * NEAR_DISTANCE;
}

bool Point::isDragged() {
	return Point::isDragMode;
}

//	méthodes spécifiques
double Point::sqrDist(const Point& p) const {
	double dx = this->x - p.x;
	double dy = this->y - p.y;
	return dx * dx + dy * dy;
}
void Point::draw(SDL_Renderer *renderer, Color c, int taille) const {
	SDL_Rect rect;
	rect.w = taille;
	rect.h = taille;
	rect.x = (int)(x - taille / 2.0);
	rect.y = (int)(y - taille / 2.0);

	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderFillRect(renderer, &rect);
}

void Point::drawCircle(SDL_Renderer *renderer, int rayon, Color c, bool continu) const {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

	//	compute points
	int nbPoints = rayon;
	double delta = 2 * M_PI / rayon;	//	1/r to have exactly one pixel each cycle of the loop
	SDL_Point *tabPoints = new SDL_Point[(long long)nbPoints + 1];
	double alpha = 0;
	for (int i = 0; i < nbPoints; i++) {
		tabPoints[i].x = (int)(this->x + rayon * cos(alpha) + 0.5);
		tabPoints[i].y = (int)(this->y + rayon * sin(alpha) + 0.5);
		alpha += delta;
	}

	//	draw
	if (continu) {
		tabPoints[nbPoints] = tabPoints[0];
		SDL_RenderDrawLines(renderer, tabPoints, nbPoints + 1);
	}
	else {
		SDL_RenderDrawPoints(renderer, tabPoints, nbPoints);
	}

	delete[] tabPoints;	//	free memory
}

void Point::update(SDL_Event &evt) {
	if (isMoveable) {
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);	//	getting mouse information

		//	start of drag and drop
		if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT && this->isNear(mouseX, mouseY)) {
			this->x = mouseX;
			this->y = mouseY;
			isDragMode = true;
			evt.type = 0;	//	set event to nothing
		}
		//	move point
		else if (evt.type == SDL_MOUSEMOTION && isDragMode) {
			this->x = mouseX;
			this->y = mouseY;
			evt.type = 0;	//	set event to nothing
		}
		//	end of drag and drop
		else if (evt.type == SDL_MOUSEBUTTONUP && isDragMode) {
			isDragMode = false;
			evt.type = 0;	//	set event to nothing
		}
	}
}

Point Point::operator+(const Vector & v) const {
	return Point(this->x + v.x, this->y + v.y);
}

Point Point::operator-(const Vector& v) const {
	return Point(this->x - v.x, this->y - v.y);
}

Point & Point::operator+=(const Vector & v) {
	this->x += v.x;
	this->y += v.y;

	return *this;
}

Point & Point::operator-=(const Vector & v) {
	this->x -= v.x;
	this->y -= v.y;

	return *this;
}

Vector::Vector()
{
	this->x = 0;
	this->y = 0;
}

//	****************
//	class of vectors
//	****************
//	-	constructors
Vector::Vector(double angle) : Point(cos(angle), sin(angle)) {
}

Vector::Vector(double x, double y) : Point(x, y) {
}

Vector::Vector(Point p1, Point p2) : Point(p2.x - p1.x, p2.y - p1.y) {
}

//	-	methodes
void Vector::normalize() {
	double d = this->magnitude();
	this->x /= d;
	this->y /= d;
}

double Vector::magnitude() {
	return sqrt(this->x * this->x + this->y * this->y);
}

//	-	operators
//	scalar product and division
Vector Vector::operator*(const double k) const {
	//return Vector(k * this->x, k * this->y);
	return std::move(Vector(k * this->x, k * this->y));
}

Vector Vector::operator/(const double k) const {
	return Vector(this->x / k, this->y / k);
}

Vector & Vector::operator*=(const double k) {
	this->x *= k;
	this->y *= k;

	return *this;
}

Vector & Vector::operator/=(const double k) {
	this->x /= k;
	this->y /= k;

	return *this;
}

Vector Vector::operator-() {
	return Vector(-this->x, -this->y);
}

//	addition and subtraction of 2 vectors
Vector Vector::operator+(const Vector & v2) const {
	return Vector(this->x + v2.x, this->y + v2.y);
}

Vector Vector::operator-(const Vector & v2) const {
	return Vector(this->x - v2.x, this->y - v2.y);
}

Vector & Vector::operator+=(const Vector & v2) {
	this->x += v2.x;
	this->y += v2.y;

	return *this;
}

Vector & Vector::operator-=(const Vector & v2) {
	this->x -= v2.x;
	this->y -= v2.y;

	return *this;
}

//	dot product and cross product
double Vector::operator*(const Vector & v2) const {
	return this->x*v2.x + this->y*v2.y;
}

double Vector::operator^(const Vector & v2) const {
	return this->x*v2.y - this->y*v2.x;
}

//	scalar product in regular order
Vector operator*(double k, const Vector & v) {
	return Vector(k * v.x, k* v.y);
}

Vector operator*(double k, const Vector && v) {
	return Vector(k * v.x, k* v.y);
}
