#include <iostream>
#include "GameEngine.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Pong", sf::Style::Default);

	GameEngine gameEngine(window);
	gameEngine.run();
}