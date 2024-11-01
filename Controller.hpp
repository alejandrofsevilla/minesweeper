#ifndef MINESWEEPER_CONTROLLER_HPP
#define MINESWEEPER_CONTROLLER_HPP

#include <SFML/Window/Event.hpp>

#include "Model.hpp"
#include "View.hpp"

class Controller {
public:
  Controller(View &view, Model &model);

  void onEvent(const sf::Event &event);

private:
  void onMouseLeftButtonPressedOnBarMenu();
  void onMouseLeftButtonPressedOnCell();
  void onMouseRightButtonPressedOnCell();
  void onMouseBothButtonsPressedOnCell();
  void onMouseButtonPressed(const sf::Event::MouseButtonEvent &event);
  void onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent &event);
  void onKeyPressed(const sf::Event::KeyEvent &event);

  View &m_view;
  Model &m_model;
};

#endif
