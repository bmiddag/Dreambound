#include "Game.hpp"

// Game constructor
Game::Game() {

}

bool Game::init() {
	return true;
}

void Game::loop() {
	beginStep();
	step();
	endStep();
	if (gui != NULL) {
		gui->render();
	}
}

void Game::beginStep() {
	for (std::list<Object*>::iterator iterator = beginStepList.begin(), end = beginStepList.end(); iterator != end; ++iterator) {
		(*iterator)->beginStep();
	}
}

void Game::step() {
	for (std::list<Object*>::iterator iterator = stepList.begin(), end = stepList.end(); iterator != end; ++iterator) {
		(*iterator)->step();
	}
}

void Game::endStep() {
	for (std::list<Object*>::iterator iterator = endStepList.begin(), end = endStepList.end(); iterator != end; ++iterator) {
		(*iterator)->endStep();
	}
}

void Game::cleanup() {

}

// Change event
void Game::fireStateChanged() {
	for (std::list<ChangeListener*>::iterator iterator = changeListenerList.begin(), end = changeListenerList.end(); iterator != end; ++iterator) {
		(*iterator)->stateChanged();
	}
}

// Event listener registration
void Game::registerStep(Object* object) {
	stepList.push_back(object);
}

void Game::registerBeginStep(Object* object) {
	beginStepList.push_back(object);
}

void Game::registerEndStep(Object* object) {
	endStepList.push_back(object);
}

void Game::registerChangeListener(ChangeListener* changeListener) {
	changeListenerList.push_back(changeListener);
}

void Game::registerRender(Object* object) {
	renderer->registerRender(object);
}

// Event listener unregistration
void Game::unregisterStep(Object* object) {
	stepList.remove(object);
}

void Game::unregisterBeginStep(Object* object) {
	beginStepList.remove(object);
}

void Game::unregisterEndStep(Object* object) {
	endStepList.remove(object);
}

void Game::unregisterChangeListener(ChangeListener* changeListener) {
	changeListenerList.remove(changeListener);
}

void Game::unregisterRender(Object* object) {
	renderer->unregisterRender(object);
}

// Basic components registration & unregistration
void Game::registerRenderer(Renderer* renderer) {
	this->renderer = renderer;
}

void Game::registerGUI(GUI* gui) {
	this->gui = gui;
}

void Game::registerWindow(sf::RenderWindow* window) {
	this->window = window;
}

void Game::unregisterRenderer() {
	this->renderer = NULL;
}

void Game::unregisterGUI() {
	this->gui = NULL;
}

void Game::unregisterWindow() {
	this->window = NULL;
}