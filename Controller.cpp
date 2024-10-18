#include "Controller.hpp"
#include "Model.hpp"
#include "SFML/Window/Mouse.hpp"

#include <cwctype>

namespace {
constexpr auto f_populationGenerationRate{.05};
} // namespace

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
  auto highlightedButton{m_view.highlightedButton()};
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
  auto pos{m_view.highlightedCellPos()};
  if (m_model.status() == Model::Status::Finished) {
    return;
  } else if (m_model.cells()[pos->first][pos->second].status !=
             Cell::Status::Hidden) { // TODO: This type of logic is in the Model
    return;
  }
  m_model.reveal(pos->first, pos->second);
  return;
}

void Controller::onMouseRightButtonPressed() {
  auto pos{m_view.highlightedCellPos()};
  if (!pos || m_model.status() == Model::Status::Finished) {
    return;
  }
  m_model.cycleCellStatus(pos->first, pos->second);
}

void Controller::onMouseButtonPressed(
    const sf::Event::MouseButtonEvent &event) {
  switch (event.button) {
  case sf::Mouse::Button::Left:
    if (m_view.highlightedButton() != View::Button::None) {
      return onMouseLeftButtonPressedOnBarMenu();
    } else if (m_view.highlightedCellPos()) {
      return onMouseLeftButtonPressedOnCell();
    }
    return;
  case sf::Mouse::Button::Right:
    return onMouseRightButtonPressed();
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
