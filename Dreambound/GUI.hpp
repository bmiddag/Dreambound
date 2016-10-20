#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Desktop.hpp>
#include "Renderer.hpp"
#include "Object.hpp"
#include <list>

class GUI {
private:
	sf::RenderWindow* window;
	Game* game;
	std::list<Object*> renderList;
	Renderer renderer;
	sfg::SFGUI sfgui;
	sfg::Window::Ptr GUIWindow;
	sfg::Button::Ptr button;
	sfg::Desktop desktop;
	sf::Clock clock;

public:
	GUI();
	GUI(Game* game, sf::RenderWindow* window);
	bool init();
	void render(); // renders gui and then calls render of renderer
	void cleanup();
	void registerRender(Object* object);
	void unregisterRender(Object* object);
	void handleEvent(sf::Event event);
	void onButtonClick();
};

#endif