#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include "GameModel.hpp"
#include "Renderer.hpp"

class GUI {
private:
	sf::RenderWindow window;
	GameModel gameModel;
	// list with all GUI instances to render

public:
	GUI(GameModel gameModel, sf::RenderWindow window);
	bool init();
	void render(); // renders gui and then calls render of renderer
	void cleanup();
};

#endif