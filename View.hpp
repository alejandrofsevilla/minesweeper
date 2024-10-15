#ifndef GAME_OF_LIFE_VIEW_HPP
#define GAME_OF_LIFE_VIEW_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>

#include "Model.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class View {
public:
  enum class Button { Quit, Restart, Size9x9, Size16x16, Size30x16, None };

  View(sf::RenderWindow &window, Model &model);

  Button highlightedButton() const;
  std::optional<Cell> highlightedCell() const;

  void update();
  void zoomIn();
  void zoomOut();
  void closeWindow();

private:
  enum class TextBoxStyle { Text, Button };
  void drawMainScreen();
  void drawBackground();
  void drawCells();
  void drawTopMenu();
  void drawCell(std::size_t row, std::size_t column);
  bool drawTextBox(const std::string &content, const sf::Vector2f &position,
                   float width, TextBoxStyle style);

  sf::Vector2f computeCellSize() const;
  sf::Vector2f computeCellPosition(std::size_t row, std::size_t column) const;

  Model &m_model;
  sf::RenderWindow &m_window;
  sf::Font m_font;
  Button m_highlightedButton;
  std::optional<Cell> m_highlightedCell;
  float m_zoomLevel;
};

#endif
