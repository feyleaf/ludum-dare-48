//SFML Ludum Dare 48
//By Benjamin C. Watt (feyleaf.com)
//April 23, 2021
#include "globals.h"
int main()
{
	//-------------"Global" variables----------------
	srand(time(NULL));
	int health = 100;
	float speed = 3.0f; //default = 3.0f
	float up_down_ratio = 0.5f;
	float left_right_ratio = 0.8f;
	float tmpVel;
	unsigned int gradient = 255;
	
	bool ascend = false;
	bool motion_vertical = true;
	bool motion_horizontal = false;
	bool flip = false;
	unsigned int animOffset = 0;
	unsigned int g_width = 600;
	unsigned int v_width = g_width;
	unsigned int v_height = 9 * v_width / 16;
	sf::Keyboard keys;
	sf::Clock game_clock;
		game_clock.restart();
	sf::Clock animateClock;
		animateClock.restart();
		sf::Clock fishClock;
		fishClock.restart();
	
	sf::Vector2f mouse_pointer = sf::Vector2f(0, 0);
	sf::Vector2f adjustedPosTop = sf::Vector2f(0, 0);
	sf::Vector2f adjustedPosBottom = sf::Vector2f(0, 0);
	sf::Texture _tex;
	sf::Texture diver_diving;
	sf::Texture diver_descent;
	sf::Texture background;
	sf::Image backMask;
	sf::Texture goodFishTex;
	sf::Texture badFishTex;
	sf::Texture heartOutline;
	sf::Texture heartFilling;
	sf::Sprite bkSprite;
	sf::Sprite _sprite;
	struct fish {
		sf::Sprite fishSprite;
		sf::Vector2f motion;
		bool badfish;
	};
	std::vector<fish> fishList;
	sf::Sprite heartLine;
	sf::Sprite hearts;
	heartOutline.loadFromFile("images/heart-outline.png");
	heartLine.setTexture(heartOutline);
	heartFilling.loadFromFile("images/heart-filling.png");
	hearts.setTexture(heartFilling);
	hearts.setPosition(sf::Vector2f(320.0f, 4.0f));
	heartLine.setPosition(sf::Vector2f(320.0f, 4.0f));
	//----------------------------------------
	sf::View hudView = sf::View(sf::FloatRect(0, 0, g_width, 9 * g_width / 16));
	sf::View gameView = hudView;
	//Window init
	
	sf::RenderWindow window(sf::VideoMode(g_width, 9* g_width /16), "Drowning in the Deep");

	sf::Vector2f boundingBox = { 0.0f, 0.0f };
	//sf::CircleShape cursor(4.0f);
	sf::CircleShape shape(50.0f);
	
	background.loadFromFile("images/tall-background-omg.png");
	backMask.loadFromFile("images/tall-background-mask.png");
		bkSprite.setTexture(background);
	bkSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
	//cursor.setFillColor(sf::Color::Red);
	_tex.loadFromFile("images/diver-underwater.png");
	diver_diving.loadFromFile("images/diver-diving.png");
	diver_descent.loadFromFile("images/diver-descent.png");
	goodFishTex.loadFromFile("images/good-fish.png");
	badFishTex.loadFromFile("images/bad-fish.png");

	_tex.setSmooth(false);
	
	_sprite.setOrigin(128.0f, 128.0f);
	_sprite.setTexture(_tex);
	_sprite.setColor(sf::Color::White);
	sf::Vector2f spr_pos= {460.0f, 560.0f};
	_sprite.setPosition(spr_pos);
	for (int i = 0; i < 90; i++)
	{
		fish tt;
		sf::Sprite tmp;
		float rn = 0.0f;
		if (rand() % 100 > 50)
		{
			tmp.setTexture(goodFishTex);
			rn = (rand() % 10 + 10) / 70.0f;
			tt.badfish = false;
			do {
				tmp.setPosition(rand() % background.getSize().x, rand() % (background.getSize().y-300) + 300);
			} while (calcDist(spr_pos, tmp.getPosition()) < 400.0f || ((backMask.getPixel(unsigned int(tt.fishSprite.getPosition().x), unsigned int(tt.fishSprite.getPosition().y)) == sf::Color::Black)));
		}
		else
		{
			tmp.setTexture(badFishTex);
			rn = (rand() % 10 + 10) / 50.0f;
			tt.badfish = true;
			do {
				tmp.setPosition(rand() % background.getSize().x, rand() % background.getSize().y/2 + background.getSize().y/2);
			} while (calcDist(spr_pos, tmp.getPosition()) < 400.0f || ((backMask.getPixel(unsigned int(tt.fishSprite.getPosition().x), unsigned int(tt.fishSprite.getPosition().y)) == sf::Color::Black)));
		}
		tmp.setScale(rn, rn);
		tt.fishSprite = tmp;
		float theta = (2 * PI) * (float(rand() % 360) / 360.0f);
		tt.motion = unitVector(theta);
		if (tt.motion.x < 0)
		{
			tt.fishSprite.setTextureRect(sf::IntRect(128, 0, -128, 128));
		}
		else
		{
			tt.fishSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
		}
		fishList.push_back(tt);
	}
	sf::SoundBuffer sfxBuff;
		sfxBuff.loadFromFile("sounds/coin.wav");
	sf::Sound sfxSound;
		sfxSound.setBuffer(sfxBuff);
	
	while (window.isOpen())//Main Game
	{
		window.clear(); //Init the window
		sf::Event event;
			window.draw(bkSprite);
		for (unsigned int c = 0; c < fishList.size(); c++)
		{
			window.draw(fishList[c].fishSprite);
		}
		_sprite.setScale(0.15f, 0.15f);
		_sprite.setPosition(spr_pos);
			window.draw(_sprite);
			window.setView(hudView);
			heartFilling.setSmooth(false);
			heartLine.setPosition(sf::Vector2f(100.0f, 0.0f));
			hearts.setPosition(sf::Vector2f(100.0f, 0.0f));
			hearts.move(sf::Vector2f(0.0f, (((100 - health) * 32) / 100.0f)));
			window.draw(hearts);
			window.draw(heartLine);
			window.setView(gameView);
		//window.draw(cursor);
		window.display();
		if (game_clock.getElapsedTime().asSeconds() > 0.0125f)
		{
			game_clock.restart();//reset to 0
			motion_vertical = false; //reset to false when the loop cycles
			ascend = false;
			bool up = keys.isKeyPressed(keys.W) || keys.isKeyPressed(keys.Up);
			bool down = keys.isKeyPressed(keys.S) || keys.isKeyPressed(keys.Down);
			bool left = keys.isKeyPressed(keys.A) || keys.isKeyPressed(keys.Left);
			bool right = keys.isKeyPressed(keys.D) || keys.isKeyPressed(keys.Right);

			//if (spr_pos.y > background.getSize().y * 0.5f) {
				_sprite.setColor(sf::Color(gradient, gradient, gradient, 255));
				bkSprite.setColor(sf::Color(192+gradient/4, 192+gradient/4, 192+gradient/4, 255));
			//}
			//else {
			//	_sprite.setColor(sf::Color::White);
			//}
				if (fishClock.getElapsedTime().asSeconds() > 1.5f)
				{
					fishClock.restart();
					for (unsigned int c = 0; c < fishList.size(); c++)
					{
						if (rand() % 100 > 50)
						{
							//update fish "AI"
							if (!fishList[c].badfish)
							{
								float theta = (2 * PI) * (float(rand() % 360) / 360.0f);
								fishList[c].motion = unitVector(theta);
							}
							else
							{
								if (calcDist(fishList[c].fishSprite.getPosition(), spr_pos) < 300.0f)
									fishList[c].motion = unitVector(angle(spr_pos - fishList[c].fishSprite.getPosition()));
								else
								{
									float theta = (2 * PI) * (float(rand() % 360) / 360.0f);
									fishList[c].motion = unitVector(theta);

								}
							}
							if (fishList[c].motion.x < 0)
							{
								fishList[c].fishSprite.setTextureRect(sf::IntRect(128, 0, -128, 128));
							}
							else
							{
								fishList[c].fishSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
							}
						}
					}
				}

				for (unsigned int c = 0; c < fishList.size(); c++)
				{
					unsigned int fishx = std::fabsf(fishList[c].fishSprite.getPosition().x + fishList[c].motion.x);
					unsigned int fishy = std::fabsf(fishList[c].fishSprite.getPosition().y + fishList[c].motion.y);
					if(!(backMask.getPixel(fishx, fishy) == sf::Color::Black))
						fishList[c].fishSprite.move(scalar(0.5f, fishList[c].motion));
				}
			if (up) {
				ascend = (!(left || right || down));
				tmpVel= speed * up_down_ratio;
				if (spr_pos.y - tmpVel > 560 && !(backMask.getPixel(unsigned int(spr_pos.x), unsigned int(spr_pos.y - tmpVel)) == sf::Color::Black))
				{
					spr_pos.y -= tmpVel;
					motion_vertical = false;
				}
			}
			if (down) {
				tmpVel = speed * up_down_ratio;
				if ((spr_pos.y + tmpVel < background.getSize().y-32) && !(backMask.getPixel(unsigned int(spr_pos.x), unsigned int(spr_pos.y+tmpVel))==sf::Color::Black))
				{
					spr_pos.y += tmpVel;
					motion_vertical = true;
				}
			}
			if (left) {
				tmpVel=speed * left_right_ratio;
				if (spr_pos.x - tmpVel > 32 && !(backMask.getPixel(unsigned int(spr_pos.x -tmpVel), unsigned int(spr_pos.y)) == sf::Color::Black)) {
					spr_pos.x -= tmpVel;
					motion_horizontal = true;
				}
			}
			if (right) {
				tmpVel=speed * left_right_ratio;
				if (spr_pos.x + tmpVel < background.getSize().x-32 && !(backMask.getPixel(unsigned int(spr_pos.x +tmpVel), unsigned int(spr_pos.y)) == sf::Color::Black))
				{
					spr_pos.x += tmpVel;
					motion_horizontal = false;
				}
			}
			if (motion_vertical == true) { _sprite.setTexture(diver_diving); }
			else { _sprite.setTexture(_tex); }
			flip = motion_horizontal;

			if ((left || right) && motion_vertical) {_sprite.setTexture(diver_descent);}
			/*
			sf::Mouse _mouse;
			sf::Vector2i _ipos = _mouse.getPosition(window);
			sf::Vector2f _pos = sf::Vector2f(float(_ipos.x), float(_ipos.y));
			mouse_pointer = (_pos + _sprite.getPosition() - sf::Vector2f(460.0f, 260.0f));
			cursor.setPosition(scalar(ratio*sizeScale, mouse_pointer));
			*/
			adjustedPosTop = sf::Vector2f(16+float(v_width) / 2.0f, 16+float(v_height) / 2.0f);
			adjustedPosBottom = sf::Vector2f(background.getSize().x-float(v_width) / 2.0f-16, background.getSize().y-float(v_height) / 2.0f-16);
			sf::Vector2f adjCenter = spr_pos;
			if (spr_pos.x < adjustedPosTop.x)
				adjCenter.x = adjustedPosTop.x;
			if (spr_pos.x > adjustedPosBottom.x)
				adjCenter.x = adjustedPosBottom.x;
			if (spr_pos.y < adjustedPosTop.y)
				adjCenter.y = adjustedPosTop.y;
			if (spr_pos.y > adjustedPosBottom.y)
				adjCenter.y = adjustedPosBottom.y;
			gameView = sf::View(adjCenter, sf::Vector2f(v_width, v_height));
			window.setView(gameView);
			
		}
		if (animateClock.getElapsedTime().asSeconds() > 0.333f)
		{
			animateClock.restart();
			if (animOffset == 0) { animOffset = 256; }
			else { animOffset = 0; }
		}
		if (flip) { _sprite.setTextureRect(sf::IntRect(256 + animOffset, 0, -256, 256)); }
		else { _sprite.setTextureRect(sf::IntRect(animOffset, 0, 256, 256)); }
		if (ascend)
		{
			_sprite.setTexture(diver_diving);
			_sprite.setTextureRect(sf::IntRect(animOffset, 256, 256, -256));
		}
		float tmpDepth = 255*(spr_pos.y / background.getSize().y);
		gradient = 255 - unsigned int(tmpDepth);

		for (unsigned int c = 0; c < fishList.size(); c++)
		{
			if (isCollision(spr_pos, sf::IntRect(fishList[c].fishSprite.getGlobalBounds())))
			{
				bkSprite.setColor(sf::Color::Red);
				health -= 1;
				hearts.setTextureRect(sf::IntRect(0,((100-health)*32)/100, 32, (health * 32)/100));
				//hearts.move(sf::Vector2f(0.0f, ((100 - health) * 32) / 100.0f));
				spr_pos.x -= 30;

			}
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
				gameView = sf::View(spr_pos, sf::Vector2f(v_width, v_height));
				window.setView(gameView);
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
			window.setView(hudView);
			heartLine.setPosition(sf::Vector2f(100.0f, 0.0f));
			hearts.setPosition(sf::Vector2f(100.0f, 0.0f));
			hearts.move(sf::Vector2f(0.0f, -1.0f + (((100 - health) * 32) / 100.0f)));
			window.draw(hearts);
			window.draw(heartLine);
			window.setView(gameView);
			//window.draw(cursor);
			window.display();
		}
	}
	return 0;
}