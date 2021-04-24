//SFML Ludum Dare Basecode
//By Benjamin C. Watt (feyleaf.com)
//April 11, 2021
#include "globals.h"
int main()
{
	unsigned int g_width = 600;
	unsigned int v_width = g_width;
	unsigned int v_height = 9 * v_width / 16;
	float ratio = float(v_width) / float(g_width);
	sf::RenderWindow window(sf::VideoMode(g_width, 9* g_width /16), "SFML works!");
	sf::CircleShape cursor(4.0f);
	sf::CircleShape shape(100.f);
	cursor.setFillColor(sf::Color::Blue);
	shape.setFillColor(sf::Color::Cyan);

	//sf::Image _image;
	sf::Texture _tex;
	//_image.loadFromFile("images/greenguy.png");
	_tex.loadFromFile("images/greenguy.png");
	_tex.setSmooth(false);
	sf::Sprite _sprite;
	_sprite.setTexture(_tex);
	_sprite.setColor(sf::Color::White);
	_sprite.setPosition(sf::Vector2f(60.0f, 60.0f));

	sf::SoundBuffer sfxBuff;
	sfxBuff.loadFromFile("sounds/coin.wav");
	sf::Sound sfxSound;
	sfxSound.setBuffer(sfxBuff);

	while (window.isOpen())
	{
		sf::Event event;
		ratio = float(v_width) / float(g_width);
		shape.setRadius(100.0f*ratio);
		_sprite.setScale(1.0f * ratio, 1.0f*ratio);
		_sprite.setPosition(scalar(ratio, sf::Vector2f(60.0f, 60.0f)));
		window.clear();
		window.draw(shape);
		window.draw(_sprite);
		window.draw(cursor);
		window.display();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				sfxSound.play();
			if (event.type == sf::Event::Resized)
			{
				v_height = window.getSize().y;
				v_width = window.getSize().x;
				unsigned int g_height = 9 * v_width / 16;
				window.setView(sf::View(sf::Vector2f(float(v_width/2.0f), float(v_height/2.0f)), sf::Vector2f(v_width, v_height)));
			}
			if (event.type == sf::Event::MouseMoved)
			{
				sf::Mouse _mouse;
				sf::Vector2i _ipos = _mouse.getPosition(window);
				sf::Vector2f _pos = sf::Vector2f(float(_ipos.x), float(_ipos.y));
				cursor.setPosition(_pos);
			}
			window.draw(shape);
			window.draw(_sprite);
			window.draw(cursor);
			window.display();
		}
	}
	return 0;
}