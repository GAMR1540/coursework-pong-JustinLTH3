#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "Paddle.h"
#include "Ball.h"

class GameEngine
{
private:
	sf::RenderWindow& m_window;
	sf::Font m_font;
	sf::Text m_hud;
	sf::SoundBuffer m_ballBuffer;
	sf::Sound m_ballSound;
	sf::Clock m_clock;


	Paddle m_paddle1;
	Paddle m_paddle2;
	Ball m_ball;

	int m_p1Score;
	int m_p2Score;
	static const int m_maxScore = 3;
	enum GameStates {intro, playing, gameOver};
	GameStates m_gStates;
public:
	GameEngine(sf::RenderWindow& window);
	void draw();
	void update();
	void run();
};

