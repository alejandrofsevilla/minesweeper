#ifndef MINESWEEPER_CONTROLLER_HPP
#define MINESWEEPER_CONTROLLER_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "Model.hpp"
#include "View.hpp"

class Controller {
public:
  Controller(View &view, Model &model);

  void onEvent(const sf::Event &event);

private:
  void onMouseLeftButtonPressed();
  void onMouseLeftButtonPressedOnBarMenu();
  void onMouseLeftButtonPressedOnCell();
  void onMouseRightButtonPressed();
  void onMouseButtonPressed(const sf::Event::MouseButtonEvent &event);
  void onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent &event);
  void onKeyPressed(const sf::Event::KeyEvent &event);

  View &m_view;
  Model &m_model;
};

#endif
