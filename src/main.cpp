#include "SFML/System/Angle.hpp"
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
  window.setFramerateLimit(30);
  std::vector<sf::RectangleShape> visualAnts;

  int antCount = 10000;
  al::World world(antCount);

  int index = 0;
  for (const auto &ant : world.getAnts()) {
    glm::vec2 pos = ant.position;
    sf::Vector2f sfPos(pos.x + window.getSize().x / 2.0f - 5.0f,
                       pos.y + window.getSize().y / 2.0f - 5.0f);

    sf::RectangleShape shape(sf::Vector2f(5.0f, 10.0f));
    shape.setOrigin(sf::Vector2f(2.5f, 5.0f));
    shape.setFillColor(sf::Color(0, index, index));
    shape.setPosition(sfPos);

    visualAnts.push_back(shape);
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
      visualAnts.at(i).setRotation(sf::degrees(ant.heading));
      i++;
    }

    window.clear(sf::Color(125.0f, 125.0f, 125.0f));
    for (const auto &antShape : visualAnts)
      window.draw(antShape);

    window.display();
  }

  return 0;
}
