#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Object.hpp"

class Renderer {
private:
	sf::RenderWindow canvas; // or the surface to render on inside the window
	Game game;
	// + ordered list with all instances to render

public:
	Renderer();
	Renderer(sf::RenderWindow canvas);
	bool init();
	void render();
	void cleanup();
	void registerRender(Object object);
};

#endif