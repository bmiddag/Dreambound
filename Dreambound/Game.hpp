#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "GUI.hpp"
#include "Object.hpp"
#include "ChangeListener.hpp"
#include <list>

class Game {
private:
	sf::RenderWindow* window;
	GUI* gui;
	Renderer* renderer;
	std::list<Object*> beginStepList;
	std::list<Object*> stepList;
	std::list<Object*> endStepList;
	std::list<ChangeListener*> changeListenerList;
	void fireStateChanged(); // for change events

public:
	Game();
	bool init();
	void loop(); // begin step -> step -> end step -> renderer.render
	void beginStep();
	void step();
	void endStep();
	void cleanup();

	// Event listener registration
	void registerStep(Object* object);
	void registerBeginStep(Object* object);
	void registerEndStep(Object* object);
	void registerChangeListener(ChangeListener* changeListener);
	void registerRender(Object* object); // calls register render in renderer

	// Event listener unregistration
	void unregisterStep(Object* object);
	void unregisterBeginStep(Object* object);
	void unregisterEndStep(Object* object);
	void unregisterChangeListener(ChangeListener* changeListener);
	void unregisterRender(Object* object); // calls unregister render in renderer

	// Main components registration & unregistration
	void registerRenderer(Renderer* renderer);
	void registerGUI(GUI* gui);
	void registerWindow(sf::RenderWindow* window);
	void unregisterRenderer();
	void unregisterGUI();
	void unregisterWindow();
};

#endif