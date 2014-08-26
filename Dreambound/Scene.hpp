#ifndef SCENE_HPP
#define SCENE_HPP

#include "Object.hpp"
#include "Game.hpp"
#include "ChangeListener.hpp"
#include <list>
#include <memory>

class Scene: public ChangeListener {
private:
	Game* game;
	std::list<std::unique_ptr<Object>> fixedObjectList; // Contains all game objects that should remain in memory across scene changes
	std::list<std::unique_ptr<Object>> sceneObjectList; // Contains all scene-specific objects
	std::list<ChangeListener*> changeListenerList;
	void fireStateChanged(); // for change events

public:
	Scene(Game* game);
	~Scene();
	bool init();
	bool loadScene(std::string sceneName);
	bool loadFixed();
	void unloadScene();
	void unloadFixed();
	void cleanup();

	// Event listener registration and unregistration
	void registerChangeListener(ChangeListener* changeListener);
	void unregisterChangeListener(ChangeListener* changeListener);

	// State changed
	virtual void stateChanged() override;
};

#endif