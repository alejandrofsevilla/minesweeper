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
  void drawCellButton(std::size_t row, std::size_t column);
  void drawMenuBarButton(const sf::Vector2f &position, Button buttonId,
                         ButtonStatus status);
  void fillButton(sf::RectangleShape &button, const sf::Texture &icon);
  void fillButton(sf::RectangleShape &button, const std::string &content);

  sf::RectangleShape makeButton(const sf::Vector2f &position, ButtonType type);
  sf::Color computeButtonColor(ButtonType type, ButtonStatus status) const;
  sf::Vector2f computeButtonSize(ButtonType type) const;
  sf::Vector2f computeCellSize() const;
  sf::Vector2f computeCellPosition(std::size_t row, std::size_t column) const;
  ButtonStatus computeButtonStatus(const sf::RectangleShape &button) const;

  Model &m_model;
  sf::RenderWindow &m_window;
  sf::Font m_font;
  std::optional<Button> m_highlightedButton;
  std::optional<Cell> m_highlightedCell; // TODO: maybe a pair<size_t, size_t>
  float m_zoomLevel;
};

#endif
