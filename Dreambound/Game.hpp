#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "GUI.hpp"

class Game {
private:
	sf::RenderWindow window;
	GUI gui;
	Renderer renderer;
	// + ordered list with all begin step listeners
	// + ordered list with all step listeners
	// + ordered list with all end step listeners

	void fireStateChanged(); // for change events

public:
	Game();
	bool init();
	void loop(); // begin step -> step -> end step -> renderer.render
	void beginStep();
	void step();
	void endStep();
	void cleanup();

	// Events
	void registerStep(Object object);
	void registerBeginStep(Object object);
	void registerEndStep(Object object);
	void registerChangeListener(Object object);
	void registerRender(Object object); // calls register render in renderer
};

#endif