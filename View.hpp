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
  enum class Button { Quit, Restart, Size, None };

  View(sf::RenderWindow &window, Model &model);

  Button buttonUnderMouse() const;
  std::optional<std::pair<int, int>> cellUnderMouse() const;

  void update();
  void zoomIn();
  void zoomOut();
  void closeWindow();

private:
  using ButtonArea = sf::RectangleShape;

  enum class ButtonType {
    Button11Middle,
    Button13Middle,
    Button3Left,
    Button2Middle,
    Button1Right,
    Standard
  };
  enum class ButtonStatus { Released, Highlighted, Pressed };
  enum class ButtonIcon {
    ButtonStandard,
    Button11Middle,
    Button13Middle,
    Button2Middle,
    Button3Left,
    Button1Right,
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
  void drawMenuButton(int col, int width, Button button, ButtonType type);
  void drawMenuDisplay(int col, int width, const std::string &content);
  void drawIconOnButton(ButtonArea &button, ButtonIcon icon);
  void drawTextOnButton(ButtonArea &button, const std::string &content);
  void scaleWindow();

  ButtonArea makeButtonArea(const sf::Vector2f &pos, int width,
                            float outlineThickness) const;
  sf::Color buttonColor(ButtonStatus status) const;
  sf::Vector2f cellButtonSize() const;
  sf::Vector2f cellButtonPosition(int col, int row) const;
  ButtonStatus buttonStatus(const ButtonArea &area) const;
  ButtonStatus menuButtonStatus(const ButtonArea &area, Button button);
  ButtonStatus cellButtonStatus(const ButtonArea &area, const Cell &cell);
  std::string buttonContent(View::Button button) const;
  ButtonIcon cellButtonIcon(const Cell &cell) const;
  ButtonIcon menuButtonIcon(ButtonType type) const;

  Model &m_model;
  sf::RenderWindow &m_window;
  sf::Font m_font;
  std::map<ButtonIcon, sf::Texture> m_icons;
  Button m_buttonUnderMouse;
  std::optional<std::pair<int, int>> m_cellUnderMouse;
  float m_zoomLevel;
};

#endif
