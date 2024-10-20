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
  enum class Button { Quit, Restart, Size9x9, Size16x16, Size30x16, None };

  View(sf::RenderWindow &window, Model &model);

  Button buttonUnderMouse() const;
  std::optional<std::pair<int, int>> cellUnderMouse() const;

  void update();
  void zoomIn();
  void zoomOut();
  void closeWindow();

private:
  using ButtonArea = sf::RectangleShape;

  enum class ButtonType { Menu, Cell };
  enum class ButtonStatus { Released, Highlighted, Pressed };
  enum class ButtonIcon {
    Cell,
    Button,
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
    Happy,
    Sad,
    Quit,
    None
  };

  void loadResources();
  void drawBackground();
  void drawCells();
  void drawMenu();
  void drawCellButton(int col, int row);
  void drawMenuButton(const sf::Vector2f &pos, Button button);
  void drawMenuDisplay(const sf::Vector2f &pos, const std::string &content);
  void drawIconOnButton(ButtonArea &button, ButtonIcon icon);
  void drawTextOnButton(ButtonArea &button, const std::string &content);

  ButtonArea makeButtonArea(const sf::Vector2f &pos, ButtonType type) const;
  float buttonOutlineThickness(ButtonType type) const;
  sf::Color buttonColor(ButtonType type, ButtonStatus status) const;
  sf::Vector2f buttonSize(ButtonType type) const;
  sf::Vector2f cellButtonSize() const;
  sf::Vector2f cellButtonPosition(int col, int row) const;
  ButtonStatus buttonStatus(const ButtonArea &area) const;
  ButtonStatus menuButtonStatus(const ButtonArea &area, Button button);
  ButtonStatus cellButtonStatus(const ButtonArea &area, const Cell &cell);
  std::string buttonContent(View::Button button) const;
  ButtonIcon cellButtonIcon(const Cell &cell) const;

  Model &m_model;
  sf::RenderWindow &m_window;
  sf::Font m_font;
  std::map<ButtonIcon, sf::Texture> m_icons;
  Button m_buttonUnderMouse;
  std::optional<std::pair<int, int>> m_cellUnderMouse;
  float m_zoomLevel;
};

#endif
