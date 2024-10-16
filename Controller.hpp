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
  void onMouseButtonPressed(const sf::Event::MouseButtonEvent &event);
  void onMouseWheelScrolled(const sf::Event::MouseWheelScrollEvent &event);
  void onMouseMoved(const sf::Event::MouseMoveEvent &event);
  void onKeyPressed(const sf::Event::KeyEvent &event);
  void onMouseButtonPressedOnCell(const Cell &cell);

  View &m_view;
  Model &m_model;
  sf::Vector2i m_mouseReferencePosition;
  bool m_isSaveFileMenuReady;
};

#endif
