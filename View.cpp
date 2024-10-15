#include "View.hpp"
#include "Cell.hpp"
#include "Model.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Shape.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <future>
#include <sstream>
#include <thread>

namespace {
constexpr auto f_fontPath{"../resources/futura.ttf"};
constexpr auto f_fontSize{18};
constexpr auto f_minZoomLevel{1.f};
constexpr auto f_maxZoomLevel{2.f};
constexpr auto f_zoomSensibility{0.1f};
constexpr auto f_defaultZoomLevel{f_maxZoomLevel};
constexpr auto f_buttonOutlineThickness{1.f};
constexpr auto f_menuButtonHeight{57.f};
constexpr auto f_menuButtonTextVPosition{18.f};
const auto f_textColor{sf::Color::White};
const auto f_buttonColor{sf::Color{60, 60, 60}};
const auto f_pressedButtonColor{sf::Color{30, 30, 30}};
const auto f_highlightedButtonColor{sf::Color{80, 80, 80}};
const auto f_barButtonColor{sf::Color{80, 80, 80}};
const auto f_barHighlightedButtonColor{sf::Color{100, 100, 100}};
const auto f_buttonOutlineColor{sf::Color::Transparent};
const auto f_backgroundColor{sf::Color{40, 40, 40}};
} // namespace

View::View(sf::RenderWindow &window, Model &model)
    : m_model{model}, m_window{window}, m_font{}, m_highlightedButton{},
      m_highlightedCell{}, m_zoomLevel{f_defaultZoomLevel} {
  m_font.loadFromFile(f_fontPath);
}

std::optional<View::Button> View::highlightedButton() const {
  return m_highlightedButton;
}

std::optional<Cell> View::highlightedCell() const { return m_highlightedCell; }

void View::update() {
  m_window.clear();
  m_highlightedButton.reset();
  m_highlightedCell.reset();
  drawBackground();
  drawTopMenu();
  drawCells();
  m_window.display();
}

void View::zoomIn() {
  m_zoomLevel = std::min(m_zoomLevel + f_zoomSensibility, f_maxZoomLevel);
}

void View::zoomOut() {
  m_zoomLevel = std::max(m_zoomLevel - f_zoomSensibility, f_minZoomLevel);
}

void View::closeWindow() { m_window.close(); }

void View::drawBackground() {
  auto windowSize{m_window.getSize()};
  sf::RectangleShape background{
      {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)}};
  background.setFillColor(f_backgroundColor);
  m_window.draw(background);
}

void View::drawCells() {
  for (decltype(m_model.height()) row = 0; row < m_model.height(); row++) {
    for (decltype(m_model.width()) col = 0; col < m_model.width(); col++) {
      drawCell(col, row);
    }
  }
}

bool View::drawMenuButton(const std::string &content,
                          const sf::Vector2f &position, float width,
                          ButtonStyle style) {
  auto highlighted{false};
  sf::RectangleShape rect{{width - 2 * f_buttonOutlineThickness,
                           f_menuButtonHeight - 2 * f_buttonOutlineThickness}};
  rect.setPosition(position.x + f_buttonOutlineThickness,
                   position.y + f_buttonOutlineThickness);
  rect.setOutlineThickness(f_buttonOutlineThickness);
  rect.setOutlineColor(f_buttonOutlineColor);
  switch (style) {
  case ButtonStyle::Button:
    if (rect.getGlobalBounds().contains(
            m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)))) {
      highlighted = true;
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        rect.setFillColor(f_pressedButtonColor);
      } else {
        rect.setFillColor(f_barHighlightedButtonColor);
      }
    } else {
      rect.setFillColor(f_barButtonColor);
    }
    break;
  case ButtonStyle::Text:
  default:
    rect.setFillColor(f_pressedButtonColor);
    break;
  }
  m_window.draw(rect);
  sf::Text text{content, m_font};
  text.setCharacterSize(f_fontSize);
  text.setPosition(position.x + (width - text.getLocalBounds().width) * .5f,
                   position.y + f_menuButtonTextVPosition);
  text.setFillColor(f_textColor);
  m_window.draw(text);
  return highlighted;
}

void View::drawCell(std::size_t col, std::size_t row) {
  auto rectSize{computeCellSize()};
  auto rect{sf::RectangleShape({rectSize.x - 2 * f_buttonOutlineThickness,
                                rectSize.y - 2 * f_buttonOutlineThickness})};
  auto rectPosition{computeCellPosition(col, row)};
  rect.setPosition(rectPosition.x + f_buttonOutlineThickness,
                   rectPosition.y + f_buttonOutlineThickness);
  rect.setOutlineColor(f_buttonOutlineColor);
  rect.setOutlineThickness(f_buttonOutlineThickness);
  switch (m_model.cellStatus(col, row)) {
  case Cell::Status::Hidden: {
    if (rect.getGlobalBounds().contains(
            m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)))) {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        m_highlightedCell = {col, row};
        rect.setFillColor(f_pressedButtonColor);
        break;
      }
      rect.setFillColor(f_highlightedButtonColor);
      break;
    }
    rect.setFillColor(f_buttonColor);
    break;
  }
  case Cell::Status::Flagged:
    break;
  case Cell::Status::Suspected:
    break;
  case Cell::Status::Revealed:
    break;
  default:
    break;
  }
  m_window.draw(rect);
}

void View::drawTopMenu() {
  sf::Vector2f position{0, 0};
  auto windowSize{m_window.getSize()};
  auto maxCellSideLength{static_cast<float>(windowSize.x) /
                         static_cast<float>(m_model.width())};
  if (drawMenuButton("Quit [Esc]", position, maxCellSideLength * 3,
                     ButtonStyle::Button)) {
    m_highlightedButton = Button::Quit;
  }
  position.x += maxCellSideLength * 3;
  if (drawMenuButton("Restart", position, maxCellSideLength * 3,
                     ButtonStyle::Button)) {
    m_highlightedButton = Button::Restart;
  }
  position.x += maxCellSideLength * 3;
  if (drawMenuButton("9 x 9", position, maxCellSideLength * 3,
                     ButtonStyle::Button)) {
    m_highlightedButton = Button::Size9x9;
  }
  position.x += maxCellSideLength * 3;
  if (drawMenuButton("16 x 16", position, maxCellSideLength * 3,
                     ButtonStyle::Button)) {
    m_highlightedButton = Button::Size16x16;
  }
  position.x += maxCellSideLength * 3;
  if (drawMenuButton("30 x 16", position, maxCellSideLength * 3,
                     ButtonStyle::Text)) {
    m_highlightedButton = Button::Size30x16;
  }
  position.x = static_cast<float>(windowSize.x) - 2 * maxCellSideLength * 3;
  if (drawMenuButton("00:00", position, maxCellSideLength * 3,
                     ButtonStyle::Text)) {
  }
  position.x += maxCellSideLength * 3;
  if (drawMenuButton("99", position, maxCellSideLength * 3,
                     ButtonStyle::Text)) {
  }
}

sf::Vector2f View::computeCellSize() const {
  auto windowSize{m_window.getSize()};
  auto maxSideLength{static_cast<float>(windowSize.x) /
                     static_cast<float>(m_model.width())};
  auto sideLength{maxSideLength * (m_zoomLevel / f_maxZoomLevel)};
  return {sideLength, sideLength};
}

sf::Vector2f View::computeCellPosition(std::size_t col, std::size_t row) const {
  auto windowSize{m_window.getSize()};
  auto cellSize{computeCellSize()};
  auto gridWidth{cellSize.x * static_cast<float>(m_model.width())};
  auto gridHeight{cellSize.y * static_cast<float>(m_model.height())};
  auto topLeftCellHPos{(static_cast<float>(windowSize.x) - gridWidth) * 0.5f};
  auto menuBarSize{f_menuButtonHeight};
  auto topLeftCellVPos{
      (static_cast<float>(windowSize.y) - menuBarSize - gridHeight) * 0.5f +
      menuBarSize};
  return {topLeftCellHPos + static_cast<float>(col) * cellSize.x,
          topLeftCellVPos + static_cast<float>(row) * cellSize.y};
}
