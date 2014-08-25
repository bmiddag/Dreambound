#include <SFML/Graphics.hpp>
#include <sstream>
#include "Game.hpp"
#include "Scene.hpp"
#include "Bol.hpp"
#include "FPSCounter.hpp"

/* Game system planning:
Every object has several functions that contain game logic.
- constructor
- destructor
- begin step (called before all step functions)
- step (happens every 1/60th of a second)
- end step (called after all step functions are finished)
- draw (called every 1/60th of a second. Frameskip if the framerate drops)
Every object can have different types. Examples are:
- actor (any playable or non-playable character). Those have playable, friendly, or enemy subtype.
- solid (platforms you can walk on). These should be defined by just vector lines.
- particle (step logic might be done in draw function as it is entirely cosmetic)
There's a master object for every "scene" (= every separate environment) that controls lists of actors, solids, etc. and unloads most of these at a scene change. */
// TODO: After implementing the game system, remove FPS counter from this file and add as separate game object

// TODO: replace all pointer logic with smart pointers: http://msdn.microsoft.com/en-us/library/hh279674.aspx

int main() {
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1080, 720), "Dreambound");
	window.setFramerateLimit(60);

	// Create Game object
	Game dreambound = Game();

	// Create GUI object
	GUI gui = GUI(&dreambound, &window);

	// Create Scene object
	Scene scene = Scene(&dreambound);
	scene.loadScene("Yay");


	while (window.isOpen()) {
		// Every 1/60th second:
		// Handle window events here
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		dreambound.loop();
		window.display();
	}

	scene.cleanup();
	return 0;
}