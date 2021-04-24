//SFML Ludum Dare 48
//By Benjamin C. Watt (feyleaf.com)
//April 23, 2021
#include "globals.h"
int main()
{
	//"Global" variables
	//float sizeScale = 1.0f;
	//float scale_window = 2.5f;
	float speed = 3.0f;
	float up_down_ratio = 0.5f;
	float left_right_ratio = 0.8f;
	float tmpVel;
	sf::Vector2f adjustedPosTop = sf::Vector2f(0, 0);
	sf::Vector2f adjustedPosBottom = sf::Vector2f(0, 0);
	bool motion_vertical = true;
	bool motion_horizontal = false;
	bool flip = false;
	sf::Keyboard keys;
	sf::Clock game_clock;
		game_clock.restart();
	sf::Clock animateClock;
		animateClock.restart();
	unsigned int animOffset = 0;
	sf::Vector2f mouse_pointer = sf::Vector2f(0, 0);
	sf::Texture _tex;
	sf::Texture diver_diving;
	sf::Texture diver_descent;
	sf::Texture background;
	sf::Sprite bkSprite;
	sf::Sprite _sprite;

	//Window init
	unsigned int g_width = 600;
	unsigned int v_width = g_width;
	unsigned int v_height = 9 * v_width / 16;
	float ratio = float(v_width) / float(g_width);
	sf::RenderWindow window(sf::VideoMode(g_width, 9* g_width /16), "Drowning in the Deep");

	sf::Vector2f boundingBox = { 0.0f, 0.0f };
	sf::CircleShape cursor(4.0f);
	sf::CircleShape shape(50.0f);
	
	background.loadFromFile("images/ocean.png");
		bkSprite.setTexture(background);
	bkSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
	cursor.setFillColor(sf::Color::Red);
	_tex.loadFromFile("images/diver-underwater.png");
	diver_diving.loadFromFile("images/diver-diving.png");
	diver_descent.loadFromFile("images/diver-descent.png");

	_tex.setSmooth(false);
	
	_sprite.setOrigin(128.0f, 128.0f);
	_sprite.setTexture(_tex);
	_sprite.setColor(sf::Color::White);
	sf::Vector2f spr_pos= {460.0f, 260.0f};
		_sprite.setPosition(spr_pos);
	
	sf::SoundBuffer sfxBuff;
		sfxBuff.loadFromFile("sounds/coin.wav");
	sf::Sound sfxSound;
		sfxSound.setBuffer(sfxBuff);
	
	while (window.isOpen())//Main Game
	{
		window.clear(); //Init the window
		sf::Event event;

		ratio = float(v_width) / float(g_width);
		shape.setRadius(75.0f*ratio);
		//bkSprite.setScale(ratio*sizeScale, ratio*sizeScale);
			window.draw(bkSprite);
		
		_sprite.setScale(0.15f * ratio, 0.15f*ratio);
		_sprite.setPosition(spr_pos);
			window.draw(_sprite);
		
		window.draw(cursor);
		window.display();
		if (game_clock.getElapsedTime().asSeconds() > 0.0125f)
		{
			game_clock.restart();//reset to 0

			motion_vertical = false; //reset to false when the loop cycles
			if (keys.isKeyPressed(keys.W)) {
				tmpVel= speed * up_down_ratio;
				if (spr_pos.y - tmpVel > 0)
				{
					spr_pos.y -= tmpVel;
					motion_vertical = false;
				}
			}
			if (keys.isKeyPressed(keys.S)) {
				tmpVel = speed * up_down_ratio;
				if (spr_pos.y + tmpVel < 2000)
				{
					spr_pos.y += tmpVel;
					motion_vertical = true;
				}
			}
			if (keys.isKeyPressed(keys.A)) {
				tmpVel=speed * left_right_ratio;
				if (spr_pos.x - tmpVel > 0) {
					spr_pos.x -= tmpVel;
					motion_horizontal = true;
				}
			}
			if (keys.isKeyPressed(keys.D)) {
				tmpVel=speed * left_right_ratio;
				if (spr_pos.x + tmpVel < 4000)
				{
					spr_pos.x += tmpVel;
					motion_horizontal = false;
				}
			}
			if (motion_vertical == true) {_sprite.setTexture(diver_diving);}
			else {_sprite.setTexture(_tex);}

			flip = motion_horizontal;

			if ((keys.isKeyPressed(keys.A) || keys.isKeyPressed(keys.D)) && motion_vertical) {_sprite.setTexture(diver_descent);}
						
			
			sf::Mouse _mouse;
			sf::Vector2i _ipos = _mouse.getPosition(window);
			sf::Vector2f _pos = sf::Vector2f(float(_ipos.x), float(_ipos.y));
			mouse_pointer = (_pos + _sprite.getPosition() - sf::Vector2f(460.0f, 260.0f));
			//cursor.setPosition(scalar(ratio*sizeScale, mouse_pointer));
			adjustedPosTop = sf::Vector2f(float(v_width) / 2.0f, float(v_height) / 2.0f);
			adjustedPosBottom = sf::Vector2f(4000.0f-float(v_width) / 2.0f, 2000.0f-float(v_height) / 2.0f);
			sf::Vector2f adjCenter = spr_pos;
			if (spr_pos.x < adjustedPosTop.x)
				adjCenter.x = adjustedPosTop.x;
			if (spr_pos.x > adjustedPosBottom.x)
				adjCenter.x = adjustedPosBottom.x;
			if (spr_pos.y < adjustedPosTop.y)
				adjCenter.y = adjustedPosTop.y;
			if (spr_pos.y > adjustedPosBottom.y)
				adjCenter.y = adjustedPosBottom.y;
			window.setView(sf::View(adjCenter, sf::Vector2f(v_width, v_height)));
			
		}
		if (animateClock.getElapsedTime().asSeconds() > 0.333f)
		{
			animateClock.restart();
			if (animOffset == 0) { animOffset = 256; }
			else { animOffset = 0; }
		}
		if (flip) { _sprite.setTextureRect(sf::IntRect(256 + animOffset, 0, -256, 256)); }
		else { _sprite.setTextureRect(sf::IntRect(animOffset, 0, 256, 256)); }

		

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
				window.setView(sf::View(spr_pos, sf::Vector2f(v_width, v_height)));
				//sizeScale = v_width / g_width;
				//window.setView(sf::View(sf::Vector2f(float(v_width/(ratio*sizeScale)), float(v_height/(ratio*sizeScale))), sf::Vector2f(v_width, v_height)));
				speed = 3.0f;
			}
			if (event.type == sf::Event::MouseMoved)
			{
				sf::Mouse _mouse;
				sf::Vector2i _ipos = _mouse.getPosition(window);
				sf::Vector2f _pos = sf::Vector2f(float(_ipos.x), float(_ipos.y));
				mouse_pointer = (_pos + _sprite.getPosition() - sf::Vector2f(460.0f, 260.0f));
				//cursor.setPosition(scalar(ratio*sizeScale, mouse_pointer));
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