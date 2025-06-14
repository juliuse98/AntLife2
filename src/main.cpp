#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>

#include "world.h"
#include <ant.h>

int main() {
  el::Configurations conf("easyloggingpp.conf");
  el::Loggers::reconfigureAllLoggers(conf);

  sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1600, 1000)), "AntLife2");
  window.setFramerateLimit(10);
  std::vector<sf::CircleShape> visualAnts;

  al::World world(8);

  int index = 0;
  for (const auto &ant : world.getAnts()) {
    glm::vec2 pos = ant.position;
    sf::Vector2f sfPos(pos.x + window.getSize().x / 2.0f - 5.0f,
                       pos.y + window.getSize().y / 2.0f - 5.0f);
    sf::CircleShape circle(10.0f);
    circle.setFillColor(sf::Color(255, index, index));
    circle.setPosition(sfPos);
    visualAnts.push_back(circle);
    index++;
  }

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    world.update();

    int i = 0;
    for (const auto &ant : world.getAnts()) {
      glm::vec2 pos = ant.position;
      sf::Vector2f sfPos(pos.x + window.getSize().x / 2.0f - 5.0f,
                         pos.y + window.getSize().y / 2.0f - 5.0f);
      visualAnts.at(i).setPosition(sfPos);
      i++;
    }

    window.clear(sf::Color::Black);
    for (const auto &antShape : visualAnts)
      window.draw(antShape);

    window.display();
  }

  return 0;
}
