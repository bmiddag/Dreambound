#ifndef SCENE_HPP
#define SCENE_HPP

#include "Object.hpp"
#include "ChangeListener.hpp"
#include <list>

class Game;

class Scene: public ChangeListener {
private:
	Game* game;
	std::list<Object*> fixedObjectList; // Contains all game objects that should remain in memory across scene changes
	std::list<Object*> sceneObjectList; // Contains all scene-specific objects
	std::list<ChangeListener*> changeListenerList;
	void fireStateChanged(); // for change events

public:
	Scene();
	bool init();
	void load(bool loadFixed);
	void unload(bool unloadFixed);
	void cleanup();

	// Event listener registration
	void registerChangeListener(ChangeListener* changeListener);

	// Event listener unregistration
	void unregisterChangeListener(ChangeListener* changeListener);

	// State changed
	virtual void stateChanged() override;
};

#endif