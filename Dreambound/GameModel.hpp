#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include <SFML/Graphics.hpp>
#include "GUI.hpp"

class GameModel {
private:
	sf::RenderWindow window;
	GUI gui;
	Renderer renderer;
	// ordered list with all listeners

public:
	GameModel();
	bool init();
	void cleanup();
	// methods to register and notify listeners
};

#endif