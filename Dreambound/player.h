#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
	void setX(double x);
	void setY(double y);
	void setXVel(double xVel);
	void setYVel(double yVel);
	void step();
	double getX();
	double getY();
	double getXVel();
	double getYVel();
	Player(double x, double y);

private:
	double x;
	double y;
	double xVel;
	double yVel;
};

#endif PLAYER_H