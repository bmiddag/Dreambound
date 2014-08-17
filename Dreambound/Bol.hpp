#ifndef BOL_HPP
#define BOL_HPP

#include "Object.hpp"

class Bol : public Object {
private:
	sf::CircleShape shape;
	sf::Color color;
	double radius;
	int x;
	int y;
public:
	Bol();
	Bol(double radius, sf::Color color);
	virtual void step();
	virtual void render(sf::RenderWindow* canvas);
};
#endif