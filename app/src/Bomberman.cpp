
#include "./Bomberman.hpp"

#include <stdexcept>
#include <iostream>

Bomberman::Bomberman()
	: window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE)
{
}

Bomberman::~Bomberman()
{
}

void Bomberman::startGame()
{
	this->start();
};

void Bomberman::updateFunc()
{
	// TODO: Impliment Delta Time
	double deltaTime = 0.01; //Placeholder until deltatime is implimented

	if (!this->window.isOpen())
		this->stop();

	sf::Event event;
	while (this->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			this->stop();
			window.close();
		}
	}

	// TODO: Need a class to map key to event
	std::vector<EngineEvent> events = {EngineEvent::move_down};
	this->engine.update(deltaTime, events);

	// TODO: Remove temp player
	Player player;
	std::vector<IRenderable *> renders = {&player};
	this->renderer.render(this->window, renders);
}
