#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Object.hpp"
#include <list>

class Renderer {
private:
	sf::RenderWindow* canvas; // TODO: Change this to a smaller surface inside the window
	Game* game;
	std::list<Object*> renderList;

public:
	Renderer();
	Renderer(sf::RenderWindow* canvas);
	bool init();
	void render();
	void cleanup();
	void registerRender(Object* object);
	void unregisterRender(Object* object);
};

#endif