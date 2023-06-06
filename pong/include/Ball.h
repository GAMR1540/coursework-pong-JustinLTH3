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
	const float MAXSPEED = 700.0;
	sf::Vector2f getPosition();
	void setPosition(float x, float y);
	const sf::Vector2f GetVelocity()const { return m_velocity; }
	void setSpeed(float val);
	//update the velocity with speed
	void updateVelocity();
	/*indecate where the ball hit
		up, right: 1
		down, left: -1
		0: no bounce
	*/
	void bounce(int up, int right);
	sf::CircleShape getShape();
	void randomDirection();
	void setTexture(sf::Texture* texture);
};

