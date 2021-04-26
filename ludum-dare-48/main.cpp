//SFML Ludum Dare 48
//By Benjamin C. Watt (feyleaf.com)
//April 23, 2021
#include "globals.h"
int main()
{
	//-------------"Global" variables----------------
	srand(unsigned(time(NULL)));
	int health = 100;
	int oxygen = 100;
	int score = 0;
	int scrapGoal = 30;
	int depthLimit = 2800+75;
	int questText = 0; //0-5?

	bool startGame = true;
	bool showDepthMessage = false;
	bool ascend = false;
	bool motion_vertical = true;
	bool motion_horizontal = false;
	bool flip = false;
	bool ending = false;

	float speed = 3.0f; //default = 3.0f
	float up_down_ratio = 0.5f;
	float left_right_ratio = 0.8f;
	float tmpVel;
	float boatsway = 0.0f;

	unsigned int gradient = 255;
	unsigned int animOffset = 0;
	unsigned int g_width = 600;
	unsigned int v_width = g_width;
	unsigned int v_height = 9 * v_width / 16;
	sf::Font font;
		font.loadFromFile("fonts/font.ttf");
	sf::Text scoreboard;
	sf::Text mainmessage;
	sf::Text depthmessage;
	
	
	sf::Keyboard keys;

	sf::Clock game_clock;
		game_clock.restart();
	sf::Clock animateClock;
		animateClock.restart();
	sf::Clock fishClock;
		fishClock.restart();
	sf::Clock boatclock;

	sf::Vector2f treasurePop = sf::Vector2f(4.0f, 24.0f);
	sf::Vector2f mouse_pointer = sf::Vector2f(0, 0);
	sf::Vector2f adjustedPosTop = sf::Vector2f(0, 0);
	sf::Vector2f adjustedPosBottom = sf::Vector2f(0, 0);
	sf::Vector2f spr_pos = { 460.0f, 560.0f };

	sf::Image backMask;

	sf::Music backgroundMusic;
	backgroundMusic.openFromFile("sounds/fluid-courage.ogg");
	backgroundMusic.setLoop(true);
	backgroundMusic.setVolume(4.0f);

	sf::Texture _tex;
	sf::Texture diver_diving;
	sf::Texture diver_descent;
	sf::Texture background;
	sf::Texture foreground;
	sf::Texture goodFishTex;
	sf::Texture badFishTex;
	sf::Texture heartOutline;
	sf::Texture heartFilling;
	sf::Texture bubbleOutline;
	sf::Texture bubbleFilling;
	sf::Texture sailboatTex;
	sf::Texture scrapTex;
	sf::Texture debTex;
	sf::Texture treasureChestBase;
	sf::Texture treasureChestLid;
	sf::Texture loot;
	sf::Texture titleTex;

	sf::Sprite bkSprite;
	sf::Sprite fgSprite;
	sf::Sprite _sprite;
	sf::Sprite titleSprite;

	struct fish {
		sf::Sprite fishSprite;
		sf::Vector2f motion;
		bool badfish;
	};
	struct scrap {
		sf::Sprite scrapSprite;
		bool isActive;
	};
	struct debris {
		sf::Sprite debrisSprite;
		float amplitude;
	};
	struct shiny {
		sf::Sprite shinySprite;
		sf::Vector2f burstForce;
		bool isActive;
	};

	sf::SoundBuffer sfxBuff;
	sf::SoundBuffer powerUpBuff;
	sf::Sound sfxSound;
	sf::Sound powerUp;

	

	std::vector<fish> fishList;
	std::vector<scrap> scrapList;
	std::vector<debris> debrisList;
	std::vector<shiny> lootList;

	sf::Sprite heartLine;
	sf::Sprite hearts;
	sf::Sprite airLine;
	sf::Sprite air;
	sf::Sprite sailboat;
	sf::Sprite treasureBase;
	sf::Sprite treasureLid;
	
	bool unlockTreasure = false;
	
	//3663, 7903
	//----------------------------------------
	//Window init
	sf::View hudView = sf::View(sf::FloatRect(0.0f, 0.0f,float(g_width), float(9 * g_width) / 16.0f));
	sf::View gameView = hudView;
	sf::RenderWindow window(sf::VideoMode(g_width, 9* g_width /16), "An Unforgiving Sea");
	sf::Vector2f boundingBox = { 0.0f, 0.0f };
	sf::CircleShape shape(50.0f);
	//---------------Music---------------
	sfxBuff.loadFromFile("sounds/coin.wav");
	powerUpBuff.loadFromFile("sounds/powerup.wav");
	sfxSound.setBuffer(sfxBuff);
	sfxSound.setVolume(33.0f);
	powerUp.setBuffer(powerUpBuff);
	powerUp.setVolume(33.0f);
	
	//-----------Images/Textures---------
	scrapTex.loadFromFile("images/scrap-floating.png");
	debTex.loadFromFile("images/debris.png");
	sailboatTex.loadFromFile("images/sailboat.png");
	heartOutline.loadFromFile("images/heart-outline.png");
	heartFilling.loadFromFile("images/heart-filling.png");
	bubbleOutline.loadFromFile("images/bubble-outline.png");
	bubbleFilling.loadFromFile("images/bubble-filling.png");
	treasureChestBase.loadFromFile("images/treasure-base.png");
	treasureChestLid.loadFromFile("images/treasure-lid.png");
	loot.loadFromFile("images/treasure.png");
	background.loadFromFile("images/tall-background-omg.png");
	foreground.loadFromFile("images/tall-landmasses.png");
	backMask.loadFromFile("images/tall-background-mask.png");
	_tex.loadFromFile("images/diver-underwater.png");
	diver_diving.loadFromFile("images/diver-diving.png");
	diver_descent.loadFromFile("images/diver-descent.png");
	goodFishTex.loadFromFile("images/good-fish.png");
	badFishTex.loadFromFile("images/bad-fish.png");
	titleTex.loadFromFile("images/gamestart.png");
	
	//------Set Textures on sprites----
	bkSprite.setTexture(background);
	fgSprite.setTexture(foreground);
	sailboat.setTexture(sailboatTex);
	heartLine.setTexture(heartOutline);
	hearts.setTexture(heartFilling);
	airLine.setTexture(bubbleOutline);
	air.setTexture(bubbleFilling);
	treasureBase.setTexture(treasureChestBase);
	treasureLid.setTexture(treasureChestLid);
	titleSprite.setTexture(titleTex);

	//-----------Set Positions-----------
	hearts.setPosition(sf::Vector2f(320.0f, 4.0f));
	heartLine.setPosition(sf::Vector2f(320.0f, 4.0f));
	air.setPosition(sf::Vector2f(260.0f, 4.0f));
	airLine.setPosition(sf::Vector2f(260.0f, 4.0f));
	treasureBase.setPosition(3663.0f, 7883.0f);
	treasureLid.setPosition(3663.0f, 7883.0f);
	bkSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
	fgSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
	scoreboard.setPosition(145.0f, 0.0f);
	mainmessage.setPosition(70.0f, 300.0f);
	depthmessage.setPosition(70.0f, 285.0f);
	titleSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

	sailboatTex.setSmooth(true);
	sailboat.setTextureRect(sf::IntRect(0,0,512,512));
	sailboat.setOrigin(224, 418);
	sailboat.setScale(sf::Vector2f(0.45f, 0.45f));
	sailboat.setPosition(sf::Vector2f(190.0, 590.0f));

	scoreboard.setFont(font);
	scoreboard.setString("Score: ");
	scoreboard.setScale(sf::Vector2f(0.75f, 0.75f));

	mainmessage.setFont(font);
	mainmessage.setString("Return to the boat to upgrade my gear using scrap.");
	mainmessage.setScale(sf::Vector2f(0.5f, 0.5f));

	depthmessage.setFont(font);
	depthmessage.setString("My scuba gear cannot let me travel deeper yet.");
	depthmessage.setScale(sf::Vector2f(0.5f, 0.5f));
	
	//cursor.setFillColor(sf::Color::Red);
	_tex.setSmooth(false);
	
	_sprite.setOrigin(128.0f, 128.0f);
	_sprite.setTexture(_tex);
	_sprite.setColor(sf::Color::White);
	
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
				tmp.setPosition(rand() % background.getSize().x, rand() % (background.getSize().y-600) + 600);
			} while ((calcDist(spr_pos, tmp.getPosition()) < 400.0f) || (backMask.getPixel(unsigned int(std::fabsf(tt.fishSprite.getPosition().x)/4.0f), unsigned int(std::fabsf(tt.fishSprite.getPosition().y))/4.0f) == sf::Color::Black));
		}
		else
		{
			tmp.setTexture(badFishTex);
			rn = (rand() % 10 + 10) / 50.0f;
			tt.badfish = true;
			do {
				tmp.setPosition(rand() % background.getSize().x, rand() % background.getSize().y/2 + background.getSize().y/2);
			} while ((calcDist(spr_pos, tmp.getPosition()) < 400.0f) || (backMask.getPixel(unsigned int(std::fabsf(tt.fishSprite.getPosition().x)/4.0f), unsigned int(std::fabsf(tt.fishSprite.getPosition().y))/4.0f) == sf::Color::Black));
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

	//placing the scrap pieces
	for (int i = 0; i < 460; i++)
	{
		scrap tt;
		sf::Sprite tmp;
		float rn = 0.0f;
		tmp.setTexture(scrapTex);
		rn = (rand() % 10 + 10) / 70.0f;
		tt.isActive = true;
		do {
			tmp.setPosition(rand() % background.getSize().x, rand() % (background.getSize().y - 600) + 600);
		} while ((calcDist(spr_pos, tmp.getPosition()) < 400.0f) || (backMask.getPixel(unsigned int(std::fabsf(tt.scrapSprite.getPosition().x) / 4.0f), unsigned int(std::fabsf(tt.scrapSprite.getPosition().y)) / 4.0f) == sf::Color::Black));
		tmp.setScale(rn, rn);
		tt.scrapSprite = tmp;
		scrapList.push_back(tt);
	}
	//place debris within the background map
	for (int i = 0; i < 360; i++)
	{
		debris deb;
		sf::Sprite tmp;
		float rn = 0.0f;
		tmp.setTexture(debTex);
		rn = (rand() % 100 + 100) / 90.0f;
		int op = rand() % 4;
		tmp.setTextureRect(sf::IntRect(op * 64, 0, 64, 64));
		tmp.setOrigin(sf::Vector2f(32.0f, 32.0f));
		deb.amplitude = 2.0f * 1000.0f / float(rand() % 1000);
		tmp.setPosition(rand() % background.getSize().x, rand() % (background.getSize().y - 600) + 600);
		tmp.setColor(sf::Color(255, 255, 255, 78));
		tmp.setScale(rn, rn);
		deb.debrisSprite = tmp;
		debrisList.push_back(deb);
	}

	for (int i = 0; i < 460; i++)
	{
		shiny tt;
		sf::Sprite tmp;
		float rn = 0.0f;
		float mag = 0.0f;
		tmp.setTexture(loot);
		rn = (rand() % 10 + 10) / 10.0f;
		mag = (rand() % 300) / 10.0f;
		int frame = (rand() % 8);
		float bangle = (-PI) + PI*float(-1*(rand() % 360) / 360.0f);
		float mult = float(8 - frame) / 10.0f;
		tmp.setScale(rn*mult, rn*mult);
		tmp.setPosition(treasureLid.getPosition());
		tmp.setTextureRect(sf::IntRect(frame * 32, 0, 32, 32));
		tt.burstForce = scalar(mag, unitVector(bangle));
		tt.shinySprite = tmp;
		tt.isActive = false;
		lootList.push_back(tt);
	}

	

	while (window.isOpen())//Main Game
	{
		sf::Event event;
		if (startGame)
		{
			window.clear();
			window.setView(hudView);
			window.draw(titleSprite);
			window.display();
		}
		else
		{
			window.clear(); //Init the window
			_sprite.setPosition(spr_pos);

			window.draw(bkSprite);
			for (unsigned int c = 0; c < debrisList.size(); c++)
			{
				window.draw(debrisList[c].debrisSprite);
			}
			for (unsigned int c = 0; c < fishList.size(); c++)
			{
				window.draw(fishList[c].fishSprite);
			}
			for (unsigned int c = 0; c < scrapList.size(); c++)
			{
				if (scrapList[c].isActive)
					window.draw(scrapList[c].scrapSprite);
			}
			if (unlockTreasure)
			{
				for (unsigned int i = 0; i < lootList.size(); i++)
				{
					if (lootList[i].isActive) window.draw(lootList[i].shinySprite);
				}
			}
			window.draw(sailboat);
			if(!ending) window.draw(_sprite);
			window.draw(fgSprite);
			window.draw(treasureBase);
			window.draw(treasureLid);
			window.setView(hudView);
			window.draw(scoreboard);
			window.draw(mainmessage);
			if (showDepthMessage) window.draw(depthmessage);
			if (unlockTreasure) scoreboard.setString("Treasure: " + sf::String(std::to_string(score)));
			else scoreboard.setString("Scrap: " + sf::String(std::to_string(score)));
			//window.draw(hearts);
			//window.draw(heartLine);
			//window.draw(air);
			//window.draw(airLine);
			window.setView(gameView);
			//window.draw(cursor);
			window.display();


			_sprite.setScale(0.15f, 0.15f);
			_sprite.setPosition(spr_pos);

			if (questText == 0)
			{
				scrapGoal = 30;
				mainmessage.setString("With 30 scrap, I can upgrade my scuba gear.");
				if (score >= scrapGoal)
				{
					mainmessage.setString("I'll return to my boat and upgrade my scuba gear now.");
					if (sailboat.getGlobalBounds().intersects(_sprite.getGlobalBounds()))
					{
						score -= scrapGoal;
						questText++;
						depthLimit += 2300;
						powerUp.play();
						showDepthMessage = false;
						mainmessage.setString(" ");
					}
				}
			}

			if (questText == 1)
			{
				scrapGoal = 50;
				mainmessage.setString("With 50 scrap, I can make faster flippers.");
				if (score >= scrapGoal)
				{
					mainmessage.setString("I can return to my boat and upgrade my flippers now.");
					if (sailboat.getGlobalBounds().intersects(_sprite.getGlobalBounds()))
					{
						score -= scrapGoal;
						questText++;
						speed = 5.0f;
						powerUp.play();
						showDepthMessage = false;
						mainmessage.setString(" ");
					}
				}
			}

			if (questText == 2)
			{
				scrapGoal = 50;
				mainmessage.setString("With 50 more scrap, I can upgrade my oxygen tank.");
				if (score >= scrapGoal)
				{
					mainmessage.setString("I can return to my boat and upgrade my oxygen tank now.");
					if (sailboat.getGlobalBounds().intersects(_sprite.getGlobalBounds()))
					{
						score -= scrapGoal;
						questText++;
						oxygen += 100;
						powerUp.play();
						showDepthMessage = false;
						mainmessage.setString(" ");
					}
				}
			}

			if (questText == 3)
			{
				scrapGoal = 80;
				mainmessage.setString("With 80 scrap, I can upgrade my scuba gear.");
				if (score >= scrapGoal)
				{
					mainmessage.setString("I can return to my boat and upgrade my scuba gear now.");
					if (sailboat.getGlobalBounds().intersects(_sprite.getGlobalBounds()))
					{
						score -= scrapGoal;
						questText++;
						depthLimit += 2300;
						powerUp.play();
						showDepthMessage = false;
						mainmessage.setString(" ");
					}
				}
			}

			if (questText == 4)
			{
				scrapGoal = 80;
				mainmessage.setString("With 80 more scrap, I can repair my broken ship.");
				if (score >= scrapGoal)
				{
					mainmessage.setString("I can return to my boat and add some upgrades now.");
					if (sailboat.getGlobalBounds().intersects(_sprite.getGlobalBounds()))
					{
						score -= scrapGoal;
						questText++;
						sailboat.setTextureRect(sf::IntRect(512, 0, 512, 512));
						depthLimit += 2300;
						powerUp.play();
						showDepthMessage = false;
						mainmessage.setString(" ");
					}
				}
			}

			if (questText == 5)
			{
				scrapGoal = 9999;
				if(!unlockTreasure) mainmessage.setString("I think there was a glint of something valuable on the ocean floor");
				if (treasureBase.getGlobalBounds().intersects(_sprite.getGlobalBounds()))
				{
					unlockTreasure = true;
					mainmessage.setString("Now I can sail away on my repaired boat, and live in luxury!");
				}
				if (unlockTreasure && sailboat.getGlobalBounds().intersects(_sprite.getGlobalBounds()))
				{
					powerUp.play();
					sailboat.setTextureRect(sf::IntRect(1024, 0, 512, 512));
					showDepthMessage = false;
					questText++;
				}
			}

			if (questText == 6)
			{
				ending = true;
				mainmessage.setPosition(70.0f, 240.0f);
				mainmessage.setString("Thanks for playing LD48! This game was made by:\n* Benjamin Watt\n* Jakob Watt\n* Liam Short");
			}

			if (unlockTreasure)
			{
				for (unsigned int i = 0; i < lootList.size(); i++)
				{
					if (calcMagnitude(lootList[i].burstForce) > 0.01f)
						lootList[i].isActive = true;
					else
						lootList[i].burstForce = sf::Vector2f(0.0f, 0.0f);
					lootList[i].burstForce = scalar(0.94f, lootList[i].burstForce);
					lootList[i].shinySprite.move(lootList[i].burstForce);
					lootList[i].shinySprite.move(sf::Vector2f(0.0f, 0.022f));
				}
			}

			
			//scoreboard.setString("Scrap: " + sf::String(std::to_string(score)));
			heartFilling.setSmooth(false);
			heartLine.setPosition(sf::Vector2f(100.0f, 0.0f));
			hearts.setPosition(sf::Vector2f(100.0f, 0.0f));
			hearts.move(sf::Vector2f(0.0f, (((100 - health) * 32) / 100.0f)));
			bubbleFilling.setSmooth(false);
			airLine.setPosition(sf::Vector2f(60.0f, 0.0f));
			air.setPosition(sf::Vector2f(60.0f, 0.0f));
			air.move(sf::Vector2f(0.0f, (((100 - oxygen) * 32) / 100.0f)));
			if (game_clock.getElapsedTime().asSeconds() > 0.0125f)
			{
				boatsway = 2.0f * sinf(boatclock.getElapsedTime().asSeconds());
				sailboat.setRotation(boatsway);
				for (unsigned int c = 0; c < debrisList.size(); c++)
				{
					debrisList[c].debrisSprite.setRotation(boatsway*debrisList[c].amplitude);
				}
				game_clock.restart();//reset to 0
				motion_vertical = false; //reset to false when the loop cycles
				ascend = false;
				bool up = keys.isKeyPressed(keys.W) || keys.isKeyPressed(keys.Up);
				bool down = keys.isKeyPressed(keys.S) || keys.isKeyPressed(keys.Down);
				bool left = keys.isKeyPressed(keys.A) || keys.isKeyPressed(keys.Left);
				bool right = keys.isKeyPressed(keys.D) || keys.isKeyPressed(keys.Right);

				//if (spr_pos.y > background.getSize().y * 0.5f) {
				_sprite.setColor(sf::Color(gradient, gradient, gradient, 255));
				bkSprite.setColor(sf::Color(192 + gradient / 4, 192 + gradient / 4, 192 + gradient / 4, 255));
				fgSprite.setColor(sf::Color(192 + gradient / 4, 192 + gradient / 4, 192 + gradient / 4, 255));
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
								if (scalar(50.0f, unitVector(theta)).y < 560.0f)
									fishList[c].motion = scalar(-1.0f, unitVector(theta));
							}
							else
							{
								if (calcDist(fishList[c].fishSprite.getPosition(), spr_pos) < 300.0f)
									fishList[c].motion = unitVector(angle(spr_pos - fishList[c].fishSprite.getPosition()));
								else
								{
									float theta = (2 * PI) * (float(rand() % 360) / 360.0f);
									fishList[c].motion = unitVector(theta);
									if (scalar(50.0f, unitVector(theta)).y < 560.0f)
										fishList[c].motion = scalar(-1.0f, unitVector(theta));

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
					//unsigned int fishx = unsigned int(std::fabsf(fishList[c].fishSprite.getPosition().x + fishList[c].motion.x)/4.0f);
					//unsigned int fishy = unsigned int(std::fabsf(fishList[c].fishSprite.getPosition().y + fishList[c].motion.y)/4.0f);
					//if (!(backMask.getPixel(fishx, fishy) == sf::Color::Black))
					fishList[c].fishSprite.move(scalar(0.5f, fishList[c].motion));
					//else
					//	fishList[c].fishSprite.move(scalar(-1.0f, fishList[c].motion));
				}
				if (!ending)
				{
					if (up) {
						ascend = (!(left || right || down));
						tmpVel = speed * up_down_ratio;
						if (spr_pos.y - tmpVel > 560 && !(backMask.getPixel(unsigned int(spr_pos.x / 4.0f), unsigned int(spr_pos.y - tmpVel) / 4) == sf::Color::Black))
						{
							spr_pos.y -= tmpVel;
							motion_vertical = false;
						}
					}
					if (down) {
						tmpVel = speed * up_down_ratio;
						if ((spr_pos.y + tmpVel < background.getSize().y - 32) && !(backMask.getPixel(unsigned int(spr_pos.x / 4.0f), unsigned int(spr_pos.y + tmpVel) / 4) == sf::Color::Black))
						{
							if (spr_pos.y < float(depthLimit))
							{
								spr_pos.y += tmpVel;
								motion_vertical = true;
							}
							else
							{
								showDepthMessage = true;
							}
						}
					}
					if (left) {
						tmpVel = speed * left_right_ratio;
						if (spr_pos.x - tmpVel > 32 && !(backMask.getPixel(unsigned int(spr_pos.x - tmpVel) / 4, unsigned int(spr_pos.y / 4.0f)) == sf::Color::Black)) {
							spr_pos.x -= tmpVel;
							motion_horizontal = true;
						}
					}
					if (right) {
						tmpVel = speed * left_right_ratio;
						if (spr_pos.x + tmpVel < background.getSize().x - 32 && !(backMask.getPixel(unsigned int(spr_pos.x + tmpVel) / 4, unsigned int(spr_pos.y / 4.0f)) == sf::Color::Black))
						{
							spr_pos.x += tmpVel;
							motion_horizontal = false;
						}
					}
					if (motion_vertical == true) { _sprite.setTexture(diver_diving); }
					else { _sprite.setTexture(_tex); }
					flip = motion_horizontal;

					if ((left || right) && motion_vertical) { _sprite.setTexture(diver_descent); }
					adjustedPosTop = sf::Vector2f(16 + float(v_width) / 2.0f, 16 + float(v_height) / 2.0f);
					adjustedPosBottom = sf::Vector2f(background.getSize().x - float(v_width) / 2.0f - 16, background.getSize().y - float(v_height) / 2.0f - 16);
					sf::Vector2f adjCenter = spr_pos;
					if (spr_pos.x < adjustedPosTop.x)
						adjCenter.x = adjustedPosTop.x;
					if (spr_pos.x > adjustedPosBottom.x)
						adjCenter.x = adjustedPosBottom.x;
					if (spr_pos.y < adjustedPosTop.y)
						adjCenter.y = adjustedPosTop.y;
					if (spr_pos.y > adjustedPosBottom.y)
						adjCenter.y = adjustedPosBottom.y;
					gameView = sf::View(adjCenter, sf::Vector2f(float(v_width), float(v_height)));
					window.setView(gameView);
				}
				else
				{
					if (right)
					{
						sailboat.move(sf::Vector2f(13.5f, 0.0f));
						gameView.setCenter(sailboat.getPosition());
					}
				}
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
			float tmpDepth = 255 * (spr_pos.y / background.getSize().y);
			gradient = 255 - unsigned int(tmpDepth);

			for (unsigned int c = 0; c < fishList.size(); c++)
			{
				if (isCollision(spr_pos, sf::IntRect(fishList[c].fishSprite.getGlobalBounds())))
				{
					bkSprite.setColor(sf::Color::Red);
					health -= 1;
					hearts.setTextureRect(sf::IntRect(0, ((100 - health) * 32) / 100, 32, (health * 32) / 100));
					spr_pos.x -= 30;

				}
			}

			for (unsigned int c = 0; c < scrapList.size(); c++)
			{
				if (scrapList[c].scrapSprite.getGlobalBounds().intersects(_sprite.getGlobalBounds()) && scrapList[c].isActive)
				{
					score++;
					sfxSound.play();
					scrapList[c].isActive = false;

				}
			}

			for (unsigned int c = 0; c < lootList.size(); c++)
			{
				if (lootList[c].shinySprite.getGlobalBounds().intersects(_sprite.getGlobalBounds()) && lootList[c].isActive)
				{
					score++;
					if(score%25==0) sfxSound.play();
					lootList[c].isActive = false;

				}
			}


		}
			while (window.pollEvent(event))
			{
				_sprite.setPosition(spr_pos);
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::KeyPressed)
				{
					if (startGame)
					{
						startGame = false;
						backgroundMusic.play();
					}
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
				{
					if (backgroundMusic.getStatus() == sf::Music::Playing)
						backgroundMusic.pause();
					else
						backgroundMusic.play();
				}
				if (event.type == sf::Event::Resized)
				{
					v_height = window.getSize().y;
					v_width = window.getSize().x;
					gameView = sf::View(spr_pos, sf::Vector2f(v_width, v_height));
					window.setView(gameView);
				}
				//if (event.type == sf::Event::MouseMoved)
				//{
				//	sf::Mouse _mouse;
				//	sf::Vector2i _ipos = _mouse.getPosition(window);
				//	sf::Vector2f _pos = sf::Vector2f(float(_ipos.x), float(_ipos.y));
				//	mouse_pointer = (_pos + _sprite.getPosition() - sf::Vector2f(460.0f, 260.0f));
					//cursor.setPosition(scalar(ratio*sizeScale, mouse_pointer));
				//}
				//window.draw(shape);
				if (false) {
					window.draw(bkSprite);
					for (unsigned int c = 0; c < debrisList.size(); c++)
					{
						window.draw(debrisList[c].debrisSprite);
					}
					for (unsigned int c = 0; c < fishList.size(); c++)
					{
						window.draw(fishList[c].fishSprite);
					}
					for (unsigned int c = 0; c < scrapList.size(); c++)
					{
						if (scrapList[c].isActive)
							window.draw(scrapList[c].scrapSprite);
					}
					if (unlockTreasure)
					{
						for (unsigned int i = 0; i < lootList.size(); i++)
						{
							if (lootList[i].isActive) window.draw(lootList[i].shinySprite);
						}
					}
					window.draw(sailboat);
					window.draw(_sprite);
					window.draw(fgSprite);
					window.draw(treasureBase);
					window.draw(treasureLid);
					window.setView(hudView);
					if (unlockTreasure) scoreboard.setString("Treasure: " + sf::String(std::to_string(score)));
					else scoreboard.setString("Scrap: " + sf::String(std::to_string(score)));
					window.draw(scoreboard);
					window.draw(mainmessage);
					if (showDepthMessage) window.draw(depthmessage);
					window.setView(gameView);
					//window.draw(cursor);
					window.display();
				}
				//heartFilling.setSmooth(false);
				//heartLine.setPosition(sf::Vector2f(100.0f, 0.0f));
				//hearts.setPosition(sf::Vector2f(100.0f, 0.0f));
				//hearts.move(sf::Vector2f(0.0f, (((100 - health) * 32) / 100.0f)));
				//window.draw(hearts);
				//window.draw(heartLine);
				//bubbleFilling.setSmooth(false);
				//airLine.setPosition(sf::Vector2f(60.0f, 0.0f));
				//air.setPosition(sf::Vector2f(60.0f, 0.0f));
				//air.move(sf::Vector2f(0.0f, (((100 - oxygen) * 32) / 100.0f)));
				//window.draw(air);
				//window.draw(airLine);
			}
		
	}
	return 0;
}