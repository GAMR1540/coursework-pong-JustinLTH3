#include "Ball.h"
#include <Vector2Func.h>
#include <cmath>

Ball::Ball(sf::Vector2f position, float radius, float speed, sf::Color color)
{
	// random direction
	setSpeed(speed);
	randomDirection();

	m_shape.setRadius(radius);
	m_shape.setPosition(position);
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
}
void Ball::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
}

void Ball::move(float dt, sf::RenderWindow& window)
{
	m_shape.move(m_velocity * dt);
	//checking upper and lower boundary 
	if (m_shape.getPosition().y < getShape().getRadius())
	{
		bounce(1, 0);
	}
	else if (m_shape.getPosition().y > window.getSize().y - 1 - getShape().getRadius())
	{
		bounce(-1, 0);
	}
}

sf::Vector2f Ball::getPosition()
{
	return m_shape.getPosition();
}

void Ball::setPosition(float x, float y)
{
	m_shape.setPosition(x, y);
}

void Ball::setSpeed(float val)
{
	m_speed = val;
	updateVelocity();
}
//update the velocity with speed
void Ball::updateVelocity()
{
	m_velocity = m_speed * Vector2Func::normalize(m_velocity);
	//normalize the velocity to get the direction, then scale it with speed
}

sf::CircleShape Ball::getShape()
{
	return m_shape;
}

void Ball::randomDirection()
{
	srand((unsigned)time(NULL));
	do
	{
		m_velocity.x = (rand() % 10 + 1) * (rand() % 2 > 0 ? 1 : -1);
		m_velocity.y = (rand() % 10 + 1) * (rand() % 2 > 0 ? 1 : -1);
	} while (atan(abs(m_velocity.y / m_velocity.x)) * 180 / 3.14f > 75 || atan(abs(m_velocity.x / m_velocity.y)) * 180 / 3.14f > 75);//clamp angle
	updateVelocity();
}

void Ball::bounce(int up, int right)
{
	if (up == 1)
	{
		m_velocity.y = abs(m_velocity.y);
	}
	else if (up == -1)
	{
		m_velocity.y = -abs(m_velocity.y);
	}
	if (right == 1)
	{
		m_velocity.x = -abs(m_velocity.x);
	}
	else if (right == -1)
	{
		m_velocity.x = abs(m_velocity.x);
	}
	//increase speed when bounce
	if (m_speed < MAXSPEED && (up != 0 || right != 0))setSpeed(m_speed + 10);
}
void Ball::setTexture(sf::Texture* texture)
{
	m_shape.setTexture(texture);
}