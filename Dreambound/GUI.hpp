#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Renderer.hpp"
#include "Object.hpp"
#include <list>

class GUI {
private:
	sf::RenderWindow* window;
	Game* game;
	std::list<Object*> renderList;
	Renderer renderer;

public:
	GUI();
	GUI(Game* game, sf::RenderWindow* window);
	bool init();
	void render(); // renders gui and then calls render of renderer
	void cleanup();
	void registerRender(Object* object);
	void unregisterRender(Object* object);
};

#endif