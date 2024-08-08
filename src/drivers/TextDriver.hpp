#pragma once
#include "../../Headers/engine.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
class TextDriver : public sf::Drawable {
public:
  TextDriver();
  sf::Text m_text;
  sf::Font font;
  void add_text(std::string str, int xPos, int yPos);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};