#include <SFML/Graphics.hpp>

class Ball
{
private:
	float m_speed;
	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;
public:
	Ball(sf::Vector2f position, float radius, float speed, sf::Color color);
	void draw(sf::RenderWindow& window);
	void move(float dt, sf::RenderWindow& window);
	const float MAXSPEED = 5.0;
	sf::Vector2f getPosition();
	void setPosition(float x, float y);
	const sf::Vector2f GetVelocity()const { return m_velocity; }
	void setSpeed(float val);
	void updateVelocity();
	void bounce(int up, int right);
	sf::CircleShape getShape();
};

