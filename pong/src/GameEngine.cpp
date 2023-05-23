#include "GameEngine.h"

void NPCMove(Ball& ball, sf::RenderWindow& window, Paddle& paddle, float dt);

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
	case GameEngine::intro:
		ss << "Press the Space\nkey to start";
		break;
	case GameEngine::playing:
		ss << m_p1Score << " - " << m_p2Score;
		break;
	case GameEngine::gameOver:
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
void checkBounce(Paddle& paddle, Ball& ball)
{
	sf::FloatRect intersection;
	if (!paddle.getBounds().intersects(ball.getShape().getGlobalBounds(), intersection))
		return;
	if (intersection.height > intersection.width)
	{
		if (paddle.getShape().getPosition().x
			< ball.getShape().getPosition().x)
		{
			ball.bounce(0, -1);
			return;
		}
		else
		{
			ball.bounce(0, 1);
			return;
		}
	}
	if (intersection.height < intersection.width)
	{
		if (paddle.getShape().getPosition().y
			< ball.getShape().getPosition().y)
		{
			ball.bounce(1, 0);
			return;
		}
		else
		{
			ball.bounce(-1, 0);
			return;
		}
	}
	if (paddle.getShape().getPosition().y
		< ball.getShape().getPosition().y)
	{
		ball.bounce(1, 0);
	}
	else ball.bounce(-1, 0);
	if (paddle.getShape().getPosition().x
		< ball.getShape().getPosition().x)
	{
		ball.bounce(0, -1);
	}
	else ball.bounce(0, 1);
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
				m_gStates = GameStates::playing;
		}

		// ADD YOUR CODE HERE !!!
		if (m_gStates == GameStates::playing)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))m_paddle1.move(dt, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))m_paddle1.move(dt, m_window.getSize().y);

			NPCMove(m_ball, m_window, m_paddle2, dt);
			m_ball.move(dt, m_window);
			int m_ball_pos_x = m_ball.getPosition().x;
			if (m_ball_pos_x < 0)
			{
				m_p2Score++;
				m_ball.setPosition(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);
			}
			else if (m_ball_pos_x > m_window.getSize().x)
			{
				m_p1Score++;
				m_ball.setPosition(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);
			}

			checkBounce(m_paddle1, m_ball);
			checkBounce(m_paddle2, m_ball);
		}


		// update hud
		update();
		// draw shapes to screen
		draw();
	}
}

void NPCMove(Ball& ball, sf::RenderWindow& window, Paddle& paddle, float dt)
{
	sf::Vector2f vel = ball.GetVelocity();
	sf::Vector2f pos = ball.getPosition();
	if (vel.x < 0)return;
	if (ball.getPosition().x < window.getSize().x / 2)return;
	float y = paddle.getShape().getPosition().x - ball.getPosition().x + ball.getShape().getRadius();
	y = y / vel.x * vel.y;
	y += pos.y;
	std::cout << y << ' ' << window.getSize().y;
	while (true)
	{
		if (y < 1)
		{
			y = -y;
			continue;
		}
		if (y > window.getSize().y - 1 - ball.getShape().getRadius() * 2)
		{
			y = window.getSize().y - y + 1;
			continue;
		}
		if (y > 1 && !(y > window.getSize().y - 1 - ball.getShape().getRadius() * 2)) break;
	}
	std::cout << y << std::endl;
	if (paddle.getShape().getPosition().y > y)
	{
		paddle.move(dt, 0);
	}
	else if (paddle.getShape().getPosition().y < y)
	{
		paddle.move(dt, window.getSize().y);
	}
}
