#include <SFML/Graphics.hpp>
#include <sstream>

/* Game system planning:
Every object has several functions that contain game logic.
- constructor
- destructor
- begin step (called before all step functions)
- step (happens every 1/60th of a second)
- end step (called after all step functions are finished)
- draw (called by a separate rendering thread every 1/60th of a second. The separate thread allows for frameskip if the framerate drops) */
// TODO: After implementing the game system, remove FPS counter from this file and add as separate game object

int main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML works!");
	sf::CircleShape shape(240.f);
	shape.setFillColor(sf::Color::Green);

	sf::Font testFont;
	if (!testFont.loadFromFile("arial.ttf"))
	{
		return 0;
	}

	sf::Clock clock;
	sf::Text fpsText;
	fpsText.setColor(sf::Color::White);
	fpsText.setFont(testFont);
	float currentTime;
	float fps;
	std::ostringstream ss;

	// 1. Start separate rendering thread which executes the draw instruction once every 1/60th second

	while (window.isOpen())
	{
		// Every 1/60th second:
		// 2. Handle window events here
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		// GAME LOGIC (speed can be configured, pass supposed fps to all objects!)
		// 3. Execute Begin Step instructions (those with begin step events register to Begin Step listener)
		// 4. Execute Step instructions (those with step events register to Step listener)
		// 5. Execute End Step instructions (those with end step events register to End Step listener)

		// 6. Calculate FPS
		currentTime = clock.restart().asSeconds();
		fps = 1.f / (currentTime);
		ss << "FPS: " << fps;
		fpsText.setString(ss.str());

		window.clear();
		window.draw(shape);
		window.draw(fpsText);
		window.display();
		ss.str(std::string());
	}

	return 0;
}