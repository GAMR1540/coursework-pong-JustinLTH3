#pragma once

#include <SFML/System/Vector2.hpp>

namespace Vector2Func
{
	//return magnitude of vec
	float magnitude(sf::Vector2f& vec);
	//return a normalized vector of vec
	sf::Vector2f normalize(sf::Vector2f& vec);
}