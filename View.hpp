#ifndef MINESWEEPER_VIEW_HPP
#define MINESWEEPER_VIEW_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <optional>

#include "Model.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class View {
public:
  enum class Button { Quit, Restart, Size9x9, Size16x16, Size30x16, None };

  View(sf::RenderWindow &window, Model &model);

  Button highlightedButton() const;
  std::optional<std::pair<size_t, size_t>> highlightedCellPos() const;

  void update();
  void zoomIn();
  void zoomOut();
  void closeWindow();

private:
  using ButtonArea = sf::RectangleShape;

  enum class ButtonType { Menu, Cell };
  enum class ButtonStatus { Released, Highlighted, Pressed };
  enum class ButtonIcon {
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Mine,
    Flag,
    QuestionMark,
    None
  };

  void loadResources();
  void drawBackground();
  void drawCells();
  void drawMenu();
  void drawCellButton(size_t col, size_t row);
  void drawMenuButton(const sf::Vector2f &pos, Button button);
  void drawMenuDisplay(const sf::Vector2f &pos, const std::string &content);
  void drawIconOnButton(ButtonArea &button, ButtonIcon icon);
  void drawTextOnButton(ButtonArea &button, const std::string &content);

  ButtonArea makeButtonArea(const sf::Vector2f &pos, ButtonType type) const;
  float buttonOutlineThickness(ButtonType type) const;
  sf::Color buttonColor(ButtonType type, ButtonStatus status) const;
  sf::Vector2f buttonSize(ButtonType type) const;
  sf::Vector2f cellButtonSize() const;
  sf::Vector2f cellButtonPosition(std::size_t col, std::size_t row) const;
  ButtonStatus buttonStatus(const ButtonArea &area) const;
  ButtonStatus menuButtonStatus(const ButtonArea &area, Button button);
  ButtonStatus cellButtonStatus(const ButtonArea &area, const Cell &cell);
  std::string buttonContent(View::Button button) const;
  ButtonIcon cellButtonIcon(const Cell &cell) const;

  Model &m_model;
  sf::RenderWindow &m_window;
  sf::Font m_font;
  std::map<ButtonIcon, sf::Texture> m_icons;
  Button m_highlightedButton;
  std::optional<std::pair<size_t, size_t>> m_highlightedCellPos;
  float m_zoomLevel;
};

#endif
