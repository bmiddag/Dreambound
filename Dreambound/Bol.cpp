#include "Bol.hpp"

Bol::Bol(double radius, sf::Color color) {
	this->radius = radius;
	this->color = color;
	shape = sf::CircleShape(radius);
	shape.setFillColor(color);
}

void Bol::step() {

}

void Bol::render(sf::RenderWindow* canvas) {
	canvas->draw(shape);
}