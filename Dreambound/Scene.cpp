#include "Scene.hpp"
#include "Game.hpp"
// Objects: Find a way to remove these...
#include "Bol.hpp"
#include "FPSCounter.hpp"

// PRIVATE

void Scene::fireStateChanged() {
	for (std::list<ChangeListener*>::iterator iterator = changeListenerList.begin(), end = changeListenerList.end(); iterator != end; ++iterator) {
		(*iterator)->stateChanged();
	}
}

// PUBLIC

Scene::Scene(Game* game) {
	this->game = game;
	if (init() == true) {
		game->registerScene(this);
	} else {
		// TODO: show error, try to deallocate and quit
	}
}

Scene::~Scene() {
	cleanup();
	game->unregisterScene();
}

bool Scene::init() {
	return loadFixed();
}

bool Scene::loadScene(std::string sceneName) {
	// TODO: First, load the appropriate file containing object lists, according to the provided scene name.
	// For every object: load, register to game object, and add to sceneObjectList
	std::unique_ptr<Object> object(new Bol(240.f, sf::Color::Green));
	if (object->getEvents() & GameEvent::BeginStep) game->registerBeginStep(object.get());
	if (object->getEvents() & GameEvent::Step) game->registerStep(object.get());
	if (object->getEvents() & GameEvent::EndStep) game->registerEndStep(object.get());
	if (object->getEvents() & GameEvent::Render) game->registerRender(object.get());
	sceneObjectList.push_back(std::move(object));

	return true;
}

bool Scene::loadFixed() {
	// TODO: For every object: load, register to game object, and add to fixedObjectList
	std::unique_ptr<Object> object(new FPSCounter(sf::Color::White));
	if (object->getEvents() & GameEvent::BeginStep) game->registerBeginStep(object.get());
	if (object->getEvents() & GameEvent::Step) game->registerStep(object.get());
	if (object->getEvents() & GameEvent::EndStep) game->registerEndStep(object.get());
	if (object->getEvents() & GameEvent::Render) game->registerRender(object.get());
	sceneObjectList.push_back(std::move(object));

	return true;
}

void Scene::unloadScene() {
	while (!sceneObjectList.empty()) {
		Object* object = sceneObjectList.back().get();
		if (object->getEvents() & GameEvent::BeginStep) game->unregisterBeginStep(object);
		if (object->getEvents() & GameEvent::Step) game->unregisterStep(object);
		if (object->getEvents() & GameEvent::EndStep) game->unregisterEndStep(object);
		if (object->getEvents() & GameEvent::Render) game->unregisterRender(object);
		sceneObjectList.pop_back();
	}
}

void Scene::unloadFixed() {
	while (!fixedObjectList.empty()) {
		Object* object = fixedObjectList.back().get();
		if (object->getEvents() & GameEvent::BeginStep) game->unregisterBeginStep(object);
		if (object->getEvents() & GameEvent::Step) game->unregisterStep(object);
		if (object->getEvents() & GameEvent::EndStep) game->unregisterEndStep(object);
		if (object->getEvents() & GameEvent::Render) game->unregisterRender(object);
		fixedObjectList.pop_back();
	}
}

void Scene::cleanup() {
	unloadScene();
	unloadFixed();
}


// Change listeners
void Scene::registerChangeListener(ChangeListener* changeListener) {
	changeListenerList.push_back(changeListener);
}

void Scene::unregisterChangeListener(ChangeListener* changeListener) {
	changeListenerList.remove(changeListener);
}

void Scene::stateChanged() {
	// do some update. Is this necessary?
}