#include <Vector2Func.h>
#include <cmath>

namespace Vector2Func
{
	sf::Vector2f Vector2Func::normalize(sf::Vector2f vec)
	{
		return vec / magnitude(vec);
	}

	float Vector2Func::magnitude(sf::Vector2f vec)
	{
		return std::sqrt(vec.x * vec.x + vec.y * vec.y);
	}
}