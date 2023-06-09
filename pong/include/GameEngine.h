#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "Paddle.h"
#include "Ball.h"

class GameEngine
{
private:
	sf::RenderWindow& m_window;
	sf::Text m_hud;
	sf::Sound m_ballSound;
	//for delta time between frames
	sf::Clock m_clock;

	//player paddle
	Paddle m_paddle1;
	//npc paddle
	Paddle m_paddle2;
	Ball m_ball;

	int m_p1Score = 0;
	int m_p2Score = 0;
	static const int m_maxScore = 3;
	enum class GameStates { intro, playing, gameOver };
	GameStates m_gStates;
public:
	GameEngine(sf::RenderWindow& window);
	void draw();
	void update();
	void run();
};

