#include <pch.h>
#include <Game.h>
#include <SFML\Graphics.hpp>

namespace{
	std::unique_ptr<Game> g_game;
}

int main(){
	int w = 800;
	int h = 600;

	std::shared_ptr<sf::RenderWindow> window;
	window.reset( new sf::RenderWindow(sf::VideoMode(w, h), GAME_TITLE));
	
	g_game.reset(new Game());
	g_game->Initialize(window, w, h);

	g_game->Run();

	return 0;
}