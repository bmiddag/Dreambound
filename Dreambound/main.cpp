#include <SFML/Graphics.hpp>
#include <sstream>

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
	float lastTime = 0;
	float currentTime;
	float fps;
	std::ostringstream ss;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		currentTime = clock.restart().asSeconds();
		fps = 1.f / (currentTime - lastTime);
		lastTime = currentTime;
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