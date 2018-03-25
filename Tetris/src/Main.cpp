#include <pch.h>
#include <Game.h>
#include <SFML\Graphics.hpp>

namespace{
	std::unique_ptr<Game> g_game;
}

int main(){
	int w = 800;
	int h = 600;

	sf::RenderWindow window(sf::VideoMode(w, h), GAME_TITLE);
	
	g_game.reset(new Game());
	g_game->Initialize(std::make_shared<sf::RenderWindow>(window), w, h);

	sf::CircleShape shape(100.0f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen()){
		sf::Event event;

		g_game->getKeyboardStateTracker()->update();

		while (window.pollEvent(event)){
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				g_game->getKeyboardStateTracker()->press(event.key.code);
				break;

			case sf::Event::KeyReleased:
				g_game->getKeyboardStateTracker()->release(event.key.code);
				break;

			default:
				break;
			}
		}

		g_game->Tick();
	}

	return 0;
}