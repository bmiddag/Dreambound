#include "GUI.hpp"
#include "Game.hpp"

// Basic GUI Constructor
GUI::GUI() {
	// DROOLIE WAS HERE
}

// GUI Constructor
GUI::GUI(Game* game, sf::RenderWindow* window) {
	this->game = game;
	this->window = window;
	if (init() == true) {
		game->registerGUI(this);
		game->registerRenderer(&renderer);
		game->registerWindow(window);
	} else {
		// moeitemeter = infinity;
	}
}

// Returns true if successful
bool GUI::init() {
	// TODO: Replace window with canvas inside window... rest of the space will be used for GUI of map editor.
	// TODO: When releasing a non-dev version, all that remains is to change canvas to full window.
	renderer = Renderer(window);
	// TODO: Create rest of the map editor GUI
	return true;
}

// renders gui and then calls render of renderer
void GUI::render() {
	for (std::list<Object*>::iterator iterator = renderList.begin(), end = renderList.end(); iterator != end; ++iterator) {
		(*iterator)->render();
	}
	renderer.render();
}

void GUI::cleanup() {

}

void GUI::registerRender(Object* object) {
	renderList.push_back(object);
}

void GUI::unregisterRender(Object* object) {
	renderList.remove(object);
}

