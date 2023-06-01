#include "GameEngine.h"
#include "Vector2Func.h"



//predict where the ball goes and move the npc paddle
void NPCMove(Ball& ball, sf::RenderWindow& window, Paddle& paddle, float dt, float& prediction, bool& predicted);

//check if the ball hit the paddle. If true, call ball.bounce
bool checkBounce(Paddle& paddle, Ball& ball);
bool predicted = false;
float prediction;

GameEngine::GameEngine(sf::RenderWindow& window)
	: m_window(window),
	m_paddle1(sf::Vector2f(20, window.getSize().y / 2.f), 10, 100, sf::Color::White),
	m_paddle2(sf::Vector2f(window.getSize().x - 20.f, window.getSize().y - 100.f), 10, 100, sf::Color::White),
	m_ball(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f), 8, 400.f, sf::Color::White)
{
	m_p1Score = 0;
	m_p2Score = 0;
	m_gStates = GameStates::intro;
	m_font.loadFromFile(".\\assets\\fonts\\digital-7.ttf");
	m_hud.setFont(m_font);
	m_hud.setCharacterSize(50);
	m_hud.setFillColor(sf::Color::White);

	m_hud.setPosition((m_window.getSize().x / 2.f) - 45.f, 10);

	m_paddle1.setSpeed(1000.f);
	m_paddle2.setSpeed(1000.f);
	m_ballBuffer.loadFromFile(".\\assets\\audio\\beep.flac");
	m_ballSound.setBuffer(m_ballBuffer);
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
			ss << "Player 1 wins";
		}
		else
		{
			ss << "Player 2 wins";
		}
		break;
	default:
		break;
	}

	m_hud.setString(ss.str());
}
bool checkBounce(Paddle& paddle, Ball& ball)
{
	sf::Vector2f paddlePos = paddle.getShape().getPosition();
	sf::Vector2f ballPos = ball.getShape().getPosition();
	float ballRadius = ball.getShape().getRadius();
	sf::Vector2f distance(abs(paddlePos.x - ballPos.x), abs(paddlePos.y - ballPos.y));

	if (distance.y > (paddle.getBounds().height / 2 + ballRadius))return false;
	if (distance.x > (paddle.getBounds().width / 2 + ballRadius))return false;
	if (distance.y <= paddle.getBounds().height / 2)
	{
		ball.bounce(0, (paddlePos.x < ballPos.x) ? -1 : 1);
		return true;
	}
	if (distance.x <= paddle.getBounds().width / 2)
	{
		ball.bounce((paddlePos.y < ballPos.y ? 1 : -1), 0);
		return true;
	}

	float tempx = (distance.x - paddle.getBounds().width / 2);
	float tempy = (distance.y - paddle.getBounds().height / 2);
	//the distance from the center of circle to the corner of the paddle
	float cornerDisSqr = tempx * tempx + tempy * tempy;
	if (cornerDisSqr <= ballRadius * ballRadius)//check if the sphere hit the corner of the rectangle
	{
		ball.bounce((paddlePos.y < ballPos.y ? 1 : -1)
			, (paddlePos.x < ballPos.x ? -1 : 1));
		return true;
	}
	return false;
}
void GameEngine::run()
{
	float dt;

	while (m_window.isOpen())
	{
		dt = m_clock.restart().asSeconds();

		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) m_window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				m_window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				m_gStates = GameStates::playing;
				m_p1Score = 0;
				m_p2Score = 0;
			}
		}

		if (m_gStates == GameStates::playing)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))m_paddle1.move(dt, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))m_paddle1.move(dt, m_window.getSize().y);

			NPCMove(m_ball, m_window, m_paddle2, dt, prediction, predicted);
			m_ball.move(dt, m_window);
			int m_ball_pos_x = m_ball.getPosition().x;
			if (m_ball_pos_x < 0)
			{
				m_p2Score++;
				m_ball.setPosition(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);
				predicted = false;
				if (m_p2Score == 11)
				{
					m_gStates = GameStates::gameOver;
				}
			}
			else if (m_ball_pos_x > m_window.getSize().x)
			{
				m_p1Score++;
				m_ball.setPosition(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);
				predicted = false;
				if (m_p1Score == 11)
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
	sf::Vector2f ballVel = ball.GetVelocity();
	sf::Vector2f ballPos = ball.getPosition();
	sf::Vector2f paddlePos = paddle.getShape().getPosition();
	sf::Vector2u winSize = window.getSize();
	if (ballVel.x < 0)
	{
		predicted = false;
		return;
	}
	if (ballPos.x < winSize.x / 2)return;

	if (!predicted)
	{
		prediction = paddlePos.x - ballPos.x;
		prediction = prediction / ballVel.x * ballVel.y;
		prediction += ballPos.y;
	}
	//only predict when not predicted already
	while (!predicted)
	{
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
		if (prediction > 1 && !(prediction > winSize.y - 1 - ball.getShape().getRadius())) predicted = true;
	}
	//move towards prediction
	if (paddlePos.y > prediction)
	{
		paddle.move(dt, 0);
	}
	else if (paddlePos.y < prediction)
	{
		paddle.move(dt, winSize.y);
	}
}
