#include "FPSCounter.hpp"

FPSCounter::FPSCounter(sf::Color color) {
	fpsText.setColor(color);
	if (!testFont.loadFromFile("data/arial.ttf")) {
		// moeitemeter += 36
	}
	fpsText.setFont(testFont); // Get font from somewhere else in future
}

void FPSCounter::endStep() {
	// The FPS Calculation happens here. 
	currentTime = clock.restart().asSeconds();
	fps = 1.f / (currentTime);
	ss << "FPS: " << fps;
	fpsText.setString(ss.str());
	ss.str(std::string()); // Reset stringstream
}

void FPSCounter::render(sf::RenderWindow* canvas) {
	canvas->draw(fpsText);
}

unsigned char FPSCounter::getEvents() {
	return GameEvent::EndStep | GameEvent::Render;
}