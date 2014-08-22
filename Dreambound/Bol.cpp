#include "Bol.hpp"

Bol::Bol(double radius, sf::Color color) {
	this->radius = radius;
	this->color = color;
	shape = sf::CircleShape(radius);
	shape.setFillColor(color);

	x = 0;
	y = 0;
}

void Bol::step() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x-=5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x+=5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) y-=5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) y+=5;
	shape.setPosition(x, y);
}

void Bol::render(sf::RenderWindow* canvas) {
	canvas->draw(shape);
}