#include "GameEngine.h"
#include "Vector2Func.h"



//predict where the ball goes and move the npc paddle
void NPCMove(Ball& ball, sf::RenderWindow& window, Paddle& paddle, float dt, float& prediction, bool& predicted);

//check if the ball hit the paddle. If the ball hit the paddle, the ball bounce and return true.
bool checkBounce(Paddle& paddle, Ball& ball);
bool predicted = false;
float prediction;

void reset(int& m_p1Score, int& m_p2Score, Ball& m_ball, sf::RenderWindow& m_window);
//reset when a player scores
void roundReset(Ball& m_ball, sf::RenderWindow& m_window);

GameEngine::GameEngine(sf::RenderWindow& window)
	: m_window(window),
	m_paddle1(sf::Vector2f(20, window.getSize().y / 2.f), 10, 100, sf::Color::White),
	m_paddle2(sf::Vector2f(window.getSize().x - 20.f, window.getSize().y - 100.f), 10, 100, sf::Color::White),
	m_ball(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), 8, 400.f, sf::Color::Yellow)
{
	m_gStates = GameStates::intro;

	//load font for the hud
	sf::Font* m_font = new sf::Font();
	m_font->loadFromFile(".\\assets\\fonts\\digital-7.ttf");
	m_hud.setFont(*m_font);
	m_hud.setCharacterSize(50);
	m_hud.setFillColor(sf::Color::White);

	m_paddle1.setSpeed(1000.f);
	m_paddle2.setSpeed(1000.f);

	//load sound for the ball
	sf::SoundBuffer* m_ballBuffer = new sf::SoundBuffer();
	m_ballBuffer->loadFromFile(".\\assets\\audio\\beep.flac");
	m_ballSound.setBuffer(*m_ballBuffer);

	//load texture for the ball
	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile(".\\assets\\texture\\basketball.png");
	m_ball.setTexture(texture);
}

void GameEngine::draw()
{
	m_window.clear();
	m_paddle1.draw(m_window);
	m_paddle2.draw(m_window);
	m_ball.draw(m_window);
	m_window.draw(m_hud);
	m_window.display();
}

void GameEngine::update()
{
	// update hud
	std::stringstream ss;
	//hud display different string based on gamestate
	switch (m_gStates)
	{
	case GameStates::intro:
		ss << "Press the Space\nkey to start";
		break;
	case GameStates::playing:
		ss << m_p1Score << " - " << m_p2Score;
		break;
	case GameStates::gameOver:
		if (m_p1Score > m_p2Score)
		{
			ss << "Player 1 wins\nPress Space to restart";
		}
		else
		{
			ss << "Player 2 wins\nPress Space to restart";
		}
		break;
	default:
		break;
	}

	m_hud.setString(ss.str());//set the string of the hud

	//center the hud
	float x = m_window.getSize().x / 2 - m_hud.getGlobalBounds().width / 2;
	m_hud.setPosition(x, m_hud.getPosition().y);
}

bool checkBounce(Paddle& paddle, Ball& ball)
{
	//paddle position
	sf::Vector2f paddlePos = paddle.getShape().getPosition();
	//ball position
	sf::Vector2f ballPos = ball.getShape().getPosition();
	//ball radius
	float ballRadius = ball.getShape().getRadius();
	//x and y distance between paddle and ball, use abs() for easy comparision
	sf::Vector2f distance(abs(paddlePos.x - ballPos.x), abs(paddlePos.y - ballPos.y));

	//check if the ball touch the paddle, return false if it does not
	if (distance.y > (paddle.getBounds().height / 2 + ballRadius))return false;
	if (distance.x > (paddle.getBounds().width / 2 + ballRadius))return false;

	//check if the ball touches left or right side of paddle
	if (distance.y <= paddle.getBounds().height / 2)
	{
		//check which side of the paddle hit by the ball, and bounce.
		//if paddlePos.x<ballPos.x, right side of the ball is hit.
		ball.bounce(0, (paddlePos.x < ballPos.x) ? -1 : 1);
		return true;
	}
	//check if the ball touches top or bottom side of paddle
	if (distance.x <= paddle.getBounds().width / 2)
	{
		//check which side of the paddle hit by the ball, and bounce
		ball.bounce((paddlePos.y < ballPos.y ? 1 : -1), 0);
		return true;
	}
	//check if the sphere hit the corner of the rectangle
	//x and y distance between the corner of paddle and the ball
	float tempx = (distance.x - paddle.getBounds().width / 2);
	float tempy = (distance.y - paddle.getBounds().height / 2);
	//the distance from the center of ball to the corner of the paddle
	float cornerDisSqr = tempx * tempx + tempy * tempy;
	if (cornerDisSqr <= ballRadius * ballRadius) 
	{
		ball.bounce((paddlePos.y < ballPos.y ? 1 : -1)
			, (paddlePos.x < ballPos.x ? -1 : 1));
		return true;
	}
	return false;
}
void GameEngine::run()
{
	float dt;//frame delta time

	while (m_window.isOpen())
	{
		dt = m_clock.restart().asSeconds();

		sf::Event event;
		while (m_window.pollEvent(event))
		{
			/*
			* Close the software when closed event or press escape key.
			* Start Game when pressing space and game has not started.
			*/
			if (event.type == sf::Event::Closed) m_window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					m_window.close();
				//only start game when gstates not playing
				if (event.key.code == sf::Keyboard::Space && m_gStates != GameStates::playing)
				{
					m_gStates = GameStates::playing;
					//reset to initial state of the game.
					reset(m_p1Score, m_p2Score, m_ball, m_window);
				}
			}
		}

		if (m_gStates == GameStates::playing)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))m_paddle1.move(dt, 0);//player move upward
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))m_paddle1.move(dt, m_window.getSize().y);//player move downward

			NPCMove(m_ball, m_window, m_paddle2, dt, prediction, predicted);//move npc paddle
			m_ball.move(dt, m_window);//move the ball

			//gameover when one's scores reach maxscore
			//check if the ball pass left or right boundry
			float m_ball_pos_x = m_ball.getPosition().x;
			if (m_ball_pos_x < 0)
			{
				m_p2Score++;
				roundReset(m_ball, m_window);
				if (m_p2Score == m_maxScore)
				{
					m_gStates = GameStates::gameOver;
				}
			}
			else if (m_ball_pos_x > m_window.getSize().x)
			{
				m_p1Score++;
				roundReset(m_ball, m_window);
				if (m_p1Score == m_maxScore)
				{
					m_gStates = GameStates::gameOver;
				}
			}

			//if ball hits paddle, play the sound.
			if (checkBounce(m_paddle1, m_ball))m_ballSound.play();
			if (checkBounce(m_paddle2, m_ball))m_ballSound.play();
		}
		// update hud
		update();
		// draw shapes to screen
		draw();
	}
}

void NPCMove(Ball& ball, sf::RenderWindow& window, Paddle& paddle, float dt, float& prediction, bool& predicted)
{
	//ball velocity
	sf::Vector2f ballVel = ball.GetVelocity();
	//ball position
	sf::Vector2f ballPos = ball.getPosition();
	//paddle position
	sf::Vector2f paddlePos = paddle.getShape().getPosition();
	//window size
	sf::Vector2u winSize = window.getSize();
	if (ballVel.x < 0)//only move when ball moves towards npc
	{
		predicted = false;
		return;
	}
	//only move when the ball passed 2/3 of the screen to limit the strength of the npc
	if (ballPos.x < winSize.x / 3 * 2) return;

	//only predict when not predicted already
	if (!predicted)//calculate where the ball lands with velocity of the ball, when no reflection
		prediction = (paddlePos.x - ballPos.x) / ballVel.x * ballVel.y + ballPos.y;
	while (!predicted)
	{
		//calculate reflection
		if (prediction < 1)
		{
			prediction = 1 - prediction;
			continue;
		}
		if (prediction > winSize.y - 1 - ball.getShape().getRadius())
		{
			prediction = (winSize.y - 1 - ball.getShape().getRadius()) * 2 - prediction;
			continue;
		}
		//if ball lands within top and bottom, break the loop.
		predicted = prediction > 1 && !(prediction > winSize.y - 1 - ball.getShape().getRadius());
	}
	//move towards prediction
	if (paddlePos.y > prediction)
	{
		paddle.move(dt, 0);//move upwards
	}
	else if (paddlePos.y < prediction)
	{
		paddle.move(dt, winSize.y);//move downwards
	}
}

//reset the score and ball
void reset(int& m_p1Score, int& m_p2Score, Ball& m_ball, sf::RenderWindow& m_window)
{
	m_p1Score = 0;
	m_p2Score = 0;
	roundReset(m_ball, m_window);
}

void roundReset(Ball& m_ball, sf::RenderWindow& m_window)
{
	m_ball.setPosition(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f); //set the ball to center
	m_ball.randomDirection();//set random direction of the ball to start
	predicted = false;
	m_ball.setSpeed(400);//reset speed
}