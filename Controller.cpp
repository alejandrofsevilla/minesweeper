#include "Controller.hpp"

#include <cwctype>

namespace {
constexpr auto f_populationGenerationRate{.05};
} // namespace

Controller::Controller(View &view, Model &model)
    : m_view{view}, m_model{model}, m_mouseReferencePosition{},
      m_isSaveFileMenuReady{true} {}

void Controller::onEvent(const sf::Event &event) {
  switch (event.type) {
  default:
    return;
  case sf::Event::Closed:
    m_view.closeWindow();
    return;
  case sf::Event::MouseButtonPressed:
    onMouseButtonPressed(event.mouseButton);
    return;
  case sf::Event::MouseWheelScrolled:
    onMouseWheelScrolled(event.mouseWheelScroll);
    return;
  case sf::Event::MouseMoved:
    onMouseMoved(event.mouseMove);
    return;
  case sf::Event::KeyPressed:
    onKeyPressed(event.key);
    return;
  }
}

void Controller::onMouseButtonPressed(
    [[maybe_unused]] const sf::Event::MouseButtonEvent &event) {
  // if (event.button != sf::Mouse::Button::Left) {
  //   return;
  // }
  // auto cell{m_view.highlightedCell()};
  // if (cell) {
  //   onMouseButtonPressedOnCell({cell->col, cell->row});
  //   return;
  // }
  // auto highlightedButton{m_view.highlightedButton()};
  // if (highlightedButton == View::Button::None) {
  //   return;
  // }
  // switch (highlightedButton) {
  //   case View::Button::Quit:
  //     m_view.closeWindow();
  //     return;
  //   case View::Button::LoadFile:
  //     m_view.setScreen(View::Screen::LoadFile);
  //     return;
  //   case View::Button::SaveFile:
  //     m_view.setScreen(View::Screen::SaveFile);
  //     return;
  //   case View::Button::ZoomOut:
  //     m_view.zoomOut();
  //     return;
  //   case View::Button::ZoomIn:
  //     m_view.zoomIn();
  //     return;
  //   case View::Button::SpeedUp:
  //     m_model.speedUp();
  //     return;
  //   case View::Button::SlowDown:
  //     m_model.slowDown();
  //     return;
  //   case View::Button::Run:
  //     m_model.run();
  //     return;
  //   case View::Button::Pause:
  //     m_model.pause();
  //     return;
  //   case View::Button::GeneratePopulation:
  //     m_model.generatePopulation(f_populationGenerationRate);
  //     return;
  //   case View::Button::Reset:
  //     m_model.reset();
  //     return;
  //   case View::Button::Clear:
  //     m_model.clear();
  //     return;
  //   case View::Button::IncreaseSize:
  //     m_model.increaseSize();
  //     return;
  //   case View::Button::ReduceSize:
  //     m_model.reduceSize();
  //     return;
  //   case View::Button::EditRule:
  //     m_view.setScreen(View::Screen::EditRule);
  //     return;
  //   default:
  //     return;
  // }
}

void Controller::onMouseWheelScrolled(
    [[maybe_unused]] const sf::Event::MouseWheelScrollEvent &event) {
  if (event.delta > 0) {
    m_view.zoomIn();
  } else if (event.delta < 0) {
    m_view.zoomOut();
  }
}

void Controller::onMouseMoved(
    [[maybe_unused]] const sf::Event::MouseMoveEvent &event) {
  // if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
  //   m_view.dragView(sf::Vector2i{event.x, event.y} -
  //   m_mouseReferencePosition);
  // }
  // m_mouseReferencePosition = sf::Vector2i{event.x, event.y};
}

void Controller::onKeyPressed(
    [[maybe_unused]] const sf::Event::KeyEvent &event) {
  // switch (event.code) {
  // case sf::Keyboard::Up:
  //   if (m_model.status() != Model::Status::Stopped) {
  //     return;
  //   }
  //   m_model.increaseSize();
  //   return;
  // case sf::Keyboard::Down:
  //   if (m_model.status() != Model::Status::Stopped) {
  //     return;
  //   }
  //   m_model.reduceSize();
  //   return;
  // case sf::Keyboard::Left:
  //   m_model.slowDown();
  //   return;
  // case sf::Keyboard::Right:
  //   m_model.speedUp();
  //   return;
  // case sf::Keyboard::R:
  //   if (m_model.status() != Model::Status::Running &&
  //       m_model.status() != Model::Status::Paused) {
  //     return;
  //   }
  //   m_model.reset();
  //   return;
  // case sf::Keyboard::C:
  //   if (m_model.status() == Model::Status::Running) {
  //     return;
  //   }
  //   m_model.clear();
  //   return;
  // case sf::Keyboard::L:
  //   m_view.setScreen(View::Screen::LoadFile);
  //   return;
  // case sf::Keyboard::S:
  //   if (m_model.initialPattern().empty()) {
  //     return;
  //   }
  //   m_isSaveFileMenuReady = false;
  //   m_view.setScreen(View::Screen::SaveFile);
  //   return;
  // case sf::Keyboard::P:
  //   if (m_model.status() != Model::Status::Stopped &&
  //       m_model.status() != Model::Status::ReadyToRun) {
  //     return;
  //   }
  //   m_model.generatePopulation(f_populationGenerationRate);
  //   return;
  // case sf::Keyboard::Escape:
  //   m_view.closeWindow();
  //   return;
  // case sf::Keyboard::U:
  //   if (m_model.status() != Model::Status::Stopped &&
  //       m_model.status() != Model::Status::ReadyToRun) {
  //     return;
  //   }
  //   m_view.setScreen(View::Screen::EditRule);
  //   return;
  // case sf::Keyboard::Space:
  //   switch (m_model.status()) {
  //   case Model::Status::ReadyToRun:
  //   case Model::Status::Paused:
  //     m_model.run();
  //     return;
  //   case Model::Status::Running:
  //     m_model.pause();
  //     return;
  //   default:
  //     return;
  //   }
  // default:
  //   return;
  // }
}

void Controller::onMouseButtonPressedOnCell([[maybe_unused]] const Cell &cell) {
  // if (m_model.status() != Model::Status::Stopped &&
  //     m_model.status() != Model::Status::ReadyToRun) {
  //   return;
  // }
  // if (m_model.cellStatus(cell.col, cell.row) == Cell::Status::Alive) {
  //   m_model.removeCell(cell);
  // } else {
  //   m_model.insertCell(cell);
  // }
  // return;
}
