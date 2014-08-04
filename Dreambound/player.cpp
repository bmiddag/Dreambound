#include "player.h"

Player::Player(double x, double y) {
	this->x = x;
	this->y = y;
	xVel = 0;
	yVel = 0;
}
 
void Player::setX(double x) {
    this->x = x;
}

void Player::setY(double y) {
    this->y = y;
}

void Player::setXVel(double xVel) {
    this->xVel = xVel;
}

void Player::setYVel(double yVel) {
    this->yVel = yVel;
}
 
double Player::getX() {
	return x;
}

double Player::getY() {
	return y;
}

double Player::getXVel() {
	return xVel;
}

double Player::getYVel() {
	return yVel;
}

void Player::step() {
	x += xVel;
	y += yVel;
}