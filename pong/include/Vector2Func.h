#pragma once

#include <SFML/System/Vector2.hpp>

namespace Vector2Func
{
	float magnitude(sf::Vector2f vec);
	sf::Vector2f normalize(sf::Vector2f vec);
}