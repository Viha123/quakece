#include "./TextDriver.hpp"
TextDriver::TextDriver() {
  if (!font.loadFromFile("assets/JetBrainsMono-Regular.ttf")) {
    // error...
  }
}
void TextDriver::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(m_text);
}
void TextDriver::add_text(std::string str, int xPos, int yPos) {
  m_text.setFont(font); // font is a sf::Font

  // set the string to display
  m_text.setString(str);

  // set the character size
  m_text.setCharacterSize(24); // in pixels, not points!

  // set the color
  m_text.setFillColor(sf::Color::White);

  // set the text style
  m_text.setStyle(sf::Text::Bold | sf::Text::Underlined);

  // set position
  m_text.setPosition(xPos, yPos);
}
