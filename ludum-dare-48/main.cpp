#include <SFML/Graphics.hpp>

int main()
{
	unsigned int width = 600;
	sf::RenderWindow window(sf::VideoMode(width, 9*width/16), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Cyan);

	while (window.isOpen())
	{
		sf::Event event;

		window.clear();
		window.draw(shape);
		window.display();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			window.draw(shape);
			window.display();
		}
	}

	return 0;
}