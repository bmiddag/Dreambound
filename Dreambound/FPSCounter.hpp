#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include <sstream>
#include "Object.hpp"

class FPSCounter : public Object {
private:
	int x;
	int y;
	sf::Clock clock;
	sf::Text fpsText;
	sf::Font testFont;
	float currentTime;
	float fps;
	std::ostringstream ss;
public:
	FPSCounter(sf::Color color);
	virtual void endStep();
	virtual void render(sf::RenderWindow* canvas);
	virtual unsigned char getEvents();
};
#endif