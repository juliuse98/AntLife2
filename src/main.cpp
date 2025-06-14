#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1600, 1000)),
                          "Hello SFML + Clang!");
  sf::CircleShape shape(50.0f);
  shape.setFillColor(sf::Color::Blue);
  shape.setPosition(
      sf::Vector2f(window.getSize().x / 2.0f - shape.getRadius(),
                   window.getSize().y / 2.0f - shape.getRadius()));
  sf::Vector2f velocity(0.1f, 0.1f);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    window.clear(sf::Color::Black);
    window.draw(shape);
    window.display();
  }

  return 0;
}
