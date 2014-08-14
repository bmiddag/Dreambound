#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Renderer.hpp"

class GUI {
private:
	sf::RenderWindow window;
	// + list with all GUI instances to render

public:
	GUI(sf::RenderWindow window);
	bool init();
	void render(); // renders gui and then calls render of renderer
	void cleanup();
	void registerRender(Object object);
};

#endif