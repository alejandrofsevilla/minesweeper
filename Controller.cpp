#include "Controller.hpp"
#include "Model.hpp"

Controller::Controller(View &view, Model &model)
    : m_view{view}, m_model{model} {}

void Controller::onEvent(const sf::Event &event) {
  switch (event.type) {
  case sf::Event::Closed:
    m_view.closeWindow();
    return;
  case sf::Event::MouseButtonPressed:
    onMouseButtonPressed(event.mouseButton);
    return;
  case sf::Event::MouseWheelScrolled:
    onMouseWheelScrolled(event.mouseWheelScroll);
    return;
  case sf::Event::KeyPressed:
    onKeyPressed(event.key);
    return;
  default:
    return;
  }
}

void Controller::onMouseWheelScrolled(
    const sf::Event::MouseWheelScrollEvent &event) {
  if (event.delta > 0) {
    m_view.zoomIn();
  } else if (event.delta < 0) {
    m_view.zoomOut();
  }
}

void Controller ::onMouseLeftButtonPressedOnBarMenu() {
  auto highlightedButton{m_view.buttonUnderMouse()};
  switch (highlightedButton) {
  case View::Button::Quit:
    m_view.closeWindow();
    return;
  case View::Button::Restart:
    m_model.restart();
    return;
  case View::Button::Size9x9:
    m_model.setSize(Model::Size::Size9x9);
    return;
  case View::Button::Size16x16:
    m_model.setSize(Model::Size::Size16x16);
    return;
  case View::Button::Size30x16:
    m_model.setSize(Model::Size::Size30x16);
    return;
  default:
    return;
  }
}

void Controller ::onMouseLeftButtonPressedOnCell() {
  auto pos{m_view.cellUnderMouse()};
  if (m_model.status() == Model::Status::Finished) {
    return;
  }
  m_model.reveal(pos->first, pos->second);
  return;
}

void Controller::onMouseRightButtonPressedOnCell() {
  if (m_model.status() == Model::Status::Finished) {
    return;
  }
  auto pos{m_view.cellUnderMouse()};
  m_model.cycleCellStatus(pos->first, pos->second);
}

void Controller::onMouseBothButtonsPressedOnCell() {
  if (m_model.status() == Model::Status::Finished) {
    return;
  }
  auto pos{m_view.cellUnderMouse()};
  m_model.tryRevealNeighbours(pos->first, pos->second);
}

void Controller::onMouseButtonPressed(
    const sf::Event::MouseButtonEvent &event) {
  auto cellIsHighlighted{m_view.cellUnderMouse().has_value()};
  auto bothButtonsPressed{sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                          sf::Mouse::isButtonPressed(sf::Mouse::Right)};
  if (cellIsHighlighted && bothButtonsPressed) {
    return onMouseBothButtonsPressedOnCell();
  }
  auto buttonIsHighlighted{m_view.buttonUnderMouse() != View::Button::None};
  switch (event.button) {
  case sf::Mouse::Button::Left:
    if (buttonIsHighlighted) {
      return onMouseLeftButtonPressedOnBarMenu();
    } else if (cellIsHighlighted) {
      return onMouseLeftButtonPressedOnCell();
    }
    return;
  case sf::Mouse::Button::Right:
    if (cellIsHighlighted) {
      return onMouseRightButtonPressedOnCell();
    }
  default:
    return;
  }
}

void Controller::onKeyPressed(const sf::Event::KeyEvent &event) {
  switch (event.code) {
  case sf::Keyboard::Escape:
    m_view.closeWindow();
    return;
  default:
    return;
  }
}
