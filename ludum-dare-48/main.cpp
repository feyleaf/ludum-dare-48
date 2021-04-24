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
	//window.setFramerateLimit(120);
	//window.setMouseCursorVisible(false);
	sf::CircleShape cursor(4.0f);
	sf::CircleShape shape(50.0f);
	sf::Texture background;
	background.loadFromFile("images/ocean.png");
	sf::Sprite bkSprite;
	bkSprite.setTexture(background);
	bkSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
	//rectangle.setSize(sf::Vector2f(600.0f, 600.0f));
	cursor.setFillColor(sf::Color::Red);
	//shape.setFillColor(sf::Color::Red);
	//rectangle.setFillColor(sf::Color(0,68,129,255));

	float speed = 3.0f;
	float up_down_ratio = 0.5f;
	float left_right_ratio = 0.8f;
	bool motion_vertical = true;
	bool motion_horizontal = false;
	sf::Keyboard keys;
	sf::Clock game_clock;
	game_clock.restart();
	sf::Clock animateClock;
	animateClock.restart();
	bool flip = false;
	unsigned int animOffset = 0;
	sf::Vector2f mouse_pointer;
	mouse_pointer = sf::Vector2f(0, 0);
	
	//sf::Image _image;
	sf::Texture _tex;
	sf::Texture diver_diving;
	sf::Texture diver_descent;
	//_image.loadFromFile("images/greenguy.png");
	_tex.loadFromFile("images/diver-underwater.png");
	diver_diving.loadFromFile("images/diver-diving.png");
	diver_descent.loadFromFile("images/diver-descent.png");

	_tex.setSmooth(false);
	sf::Sprite _sprite;
	_sprite.setOrigin(128.0f, 128.0f);
	_sprite.setTexture(_tex);
	_sprite.setColor(sf::Color::White);
	sf::Vector2f spr_pos= {460.0f, 260.0f};
	_sprite.setPosition(spr_pos);
	
	sf::SoundBuffer sfxBuff;
	sfxBuff.loadFromFile("sounds/coin.wav");
	sf::Sound sfxSound;
	sfxSound.setBuffer(sfxBuff);

	while (window.isOpen())
	{
		sf::Event event;
		ratio = float(v_width) / float(g_width);
		shape.setRadius(75.0f*ratio);
		_sprite.setScale(0.25f * ratio, 0.25f*ratio);
		_sprite.setPosition(scalar(ratio, spr_pos));
		//rectangle.setPosition(0.0f, 0.0f);
		window.clear();
		//window.draw(shape);
		window.draw(bkSprite);
		_sprite.setPosition(spr_pos);
		window.draw(_sprite);
		window.draw(cursor);
		window.display();
		if (game_clock.getElapsedTime().asSeconds() > 0.0125f)
		{
			game_clock.restart();
			motion_vertical = false;
			//motion_horizontal = false;
			if (keys.isKeyPressed(keys.W)) {
				spr_pos.y -= speed * up_down_ratio;
				motion_vertical = false;

				//diver-diving
			}
			if (keys.isKeyPressed(keys.S)) {
				spr_pos.y += speed * up_down_ratio;
				motion_vertical = true;
			}
			if (keys.isKeyPressed(keys.A)) {
				spr_pos.x -= speed * left_right_ratio;
				motion_horizontal = true;
			}
			if (keys.isKeyPressed(keys.D)) {
				spr_pos.x += speed * left_right_ratio;
				motion_horizontal = false;
			}
			if (motion_vertical == true) {_sprite.setTexture(diver_diving);}
			else {_sprite.setTexture(_tex);}

			flip = motion_horizontal;

			if ((keys.isKeyPressed(keys.A) || keys.isKeyPressed(keys.D)) && motion_vertical) {_sprite.setTexture(diver_descent);}
						
			
			sf::Mouse _mouse;
			sf::Vector2i _ipos = _mouse.getPosition(window);
			sf::Vector2f _pos = sf::Vector2f(float(_ipos.x)*1.5f, float(_ipos.y)*1.5f);
			mouse_pointer = (_pos + _sprite.getPosition() - sf::Vector2f(460.0f, 260.0f));
			cursor.setPosition(mouse_pointer);
			
			window.setView(sf::View(_sprite.getPosition(), sf::Vector2f(v_width*1.5f, v_height*1.5f)));
		}
		if (animateClock.getElapsedTime().asSeconds() > 0.333f)
		{
			animateClock.restart();
			if (animOffset == 0)
				animOffset = 256;
			else
			{
				animOffset = 0;
			}
		}
		if (flip)
		{
			_sprite.setTextureRect(sf::IntRect(256 + animOffset, 0, -256, 256));

		}
		else
		{
			_sprite.setTextureRect(sf::IntRect(animOffset, 0, 256, 256));
		}

		

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
				window.setView(sf::View(sf::Vector2f(float(v_width/2.0f), float(v_height/2.0f)), sf::Vector2f(v_width*1.5f, v_height*1.5f)));
			}
			if (event.type == sf::Event::MouseMoved)
			{
				sf::Mouse _mouse;
				sf::Vector2i _ipos = _mouse.getPosition(window);
				sf::Vector2f _pos = sf::Vector2f(float(_ipos.x)*1.5f, float(_ipos.y)*1.5f);
				mouse_pointer = (_pos + _sprite.getPosition() - sf::Vector2f(460.0f, 260.0f));
				cursor.setPosition(mouse_pointer);
			}
			//window.draw(shape);
			window.draw(bkSprite);
			_sprite.setPosition(spr_pos);
			window.draw(_sprite);
			window.draw(cursor);
			window.display();
		}
	}
	return 0;
}