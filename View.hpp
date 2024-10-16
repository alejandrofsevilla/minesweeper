#ifndef MINESWEEPER_VIEW_HPP
#define MINESWEEPER_VIEW_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <optional>

#include "Model.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class View {
public:
  enum class Button { Quit, Restart, Size9x9, Size16x16, Size30x16 };

  View(sf::RenderWindow &window, Model &model);

  std::optional<Button> highlightedButton() const;
  std::optional<Cell> highlightedCell() const;

  void update();
  void zoomIn();
  void zoomOut();
  void closeWindow();

private:
  enum class ButtonType { MenuBar, Cell };
  enum class ButtonStatus { Released, Highlighted, Pressed };
  void drawBackground();
  void drawCells();
  void drawMenuBar();
  void drawButton(sf::RectangleShape &button, const sf::Texture &icon);
  void drawButton(sf::RectangleShape &button, const std::string &content);
  void drawCellButton(std::size_t row, std::size_t column);
  void drawMenuBarButton(const sf::Vector2f &position, Button buttonId,
                         ButtonStatus status);

  ButtonStatus computeButtonStatus(const sf::RectangleShape &button) const;
  sf::RectangleShape makeEmptyButton(const sf::Vector2f &position,
                                     ButtonType type);
  sf::Color computeButtonColor(ButtonType type, ButtonStatus status) const;
  sf::Vector2f computeButtonSize(ButtonType type) const;
  sf::Vector2f computeCellSize() const;
  sf::Vector2f computeCellPosition(std::size_t row, std::size_t column) const;

  Model &m_model;
  sf::RenderWindow &m_window;
  sf::Font m_font;
  std::optional<Button> m_highlightedButton;
  std::optional<Cell> m_highlightedCell;
  float m_zoomLevel;
};

#endif
