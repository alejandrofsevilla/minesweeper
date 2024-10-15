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
const auto f_frameColor{sf::Color::Black};
const auto f_livingCellColor{sf::Color::White};
const auto f_gridColor{sf::Color::Black};
const auto f_deadCellColor{sf::Color{60, 60, 60}};
const auto f_simpleTextBoxFillColor{sf::Color{20, 20, 20}};
const auto f_simpleTextBoxOutlineColor{sf::Color::Transparent};
const auto f_simpleTextBoxTextColor{sf::Color::White};
const auto f_clickedButtonFillColor{sf::Color{10, 10, 10}};
const auto f_clickedButtonOutlineColor{sf::Color::Transparent};
const auto f_clickedButtonTextColor{sf::Color::White};
const auto f_highlightedButtonFilledColor{sf::Color{70, 70, 70}};
const auto f_highlightedButtonOutlineColor{sf::Color::Transparent};
const auto f_highlightedButtonTextColor{sf::Color::White};
const auto f_buttonFillColor{sf::Color{50, 50, 50}};
const auto f_buttonOutlineColor{sf::Color::Transparent};
const auto f_buttonTextColor{sf::Color::White};
const auto f_displayTextBoxFillColor{sf::Color::Black};
const auto f_displayTextBoxOutlineColor{sf::Color{35, 35, 35}};
const auto f_displayTextBoxTextFillColor{sf::Color::White};
const auto f_hiddenTextBoxFillColor{sf::Color{35, 35, 35}};
const auto f_hiddenTextBoxOutlineColor{sf::Color{35, 35, 35}};
const auto f_hiddenTextBoxTextColor{sf::Color{100, 100, 100}};
const auto f_hiddenButtonFillColor{sf::Color{45, 45, 45}};
const auto f_hiddenButtonOutlineColor{sf::Color{35, 35, 35}};
const auto f_hiddenButtonTextColor{sf::Color{110, 110, 110}};
constexpr auto f_fontPath{"../resources/futura.ttf"};
constexpr auto f_frameHorizontalThickness{57.f};
constexpr auto f_frameVerticalThickness{0.f};
constexpr auto f_fontSize{18};
constexpr auto f_textBoxOutlineThickness{1.f};
constexpr auto f_textBoxHeight{57.f};
constexpr auto f_minZoomLevel{1.f};
constexpr auto f_maxZoomLevel{2.f};
constexpr auto f_defaultZoomLevel{f_maxZoomLevel};
constexpr auto f_zoomSensibility{0.1f};
constexpr auto f_textBoxTextVerticalPosition{18.f};
constexpr auto f_defaultButtonWidth{187.0f};
constexpr auto f_addRemoveCellTextWidth{290.f};
constexpr auto f_dragViewTextWidth{240.f};
constexpr auto f_zoomTextWidth{235.f};
constexpr auto f_displayBoxWidth{125.f};
constexpr auto f_plusMinusButtonWidth{50.f};
constexpr auto f_defaultTextWidth{195.f};
constexpr auto f_saveMenuInfoTextWidth{180.f};
constexpr auto f_pageUpDownTextWidth{370.f};
constexpr auto f_scrollUpDownTextWidth{330.f};
constexpr auto f_ruleEditBoxWidth{220.f};
constexpr auto f_editRuleMenuInfoTextWidth{120.f};
constexpr auto f_defaultScreenWidth{1920};
constexpr auto f_defaultScreenHeight{1080};
// NEW CONSTANTS:
constexpr auto f_defaultCellSize{32};
constexpr auto f_cellOutlineThickness{1.f};
const auto f_buttonColor{sf::Color{60, 60, 60}};
const auto f_pressedButtonColor{sf::Color{30, 30, 30}};
const auto f_highlightedButtonColor{sf::Color{80, 80, 80}};
const auto f_barButtonColor{sf::Color{80, 80, 80}};
const auto f_barHighlightedButtonColor{sf::Color{100, 100, 100}};
const auto f_cellOutlineColor{sf::Color::Transparent};
const auto f_backgroundColor{sf::Color{40, 40, 40}};
} // namespace

View::View(sf::RenderWindow &window, Model &model)
    : m_model{model}, m_window{window}, m_font{},
      m_highlightedButton{Button::None}, m_highlightedCell{},
      m_zoomLevel{f_defaultZoomLevel} {}

View::Button View::highlightedButton() const { return m_highlightedButton; }

std::optional<Cell> View::highlightedCell() const { return m_highlightedCell; }

void View::update() {
  m_window.clear();
  m_highlightedButton = Button::None;
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
  sf::RectangleShape background{{f_defaultScreenWidth, f_defaultScreenHeight}};
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

bool View::drawTextBox(const std::string &content, const sf::Vector2f &position,
                       float width, TextBoxStyle style) {
  auto highlighted{false};
  sf::RectangleShape rect{{width - 2 * f_textBoxOutlineThickness,
                           f_textBoxHeight - 2 * f_textBoxOutlineThickness}};
  rect.setPosition(position.x + f_textBoxOutlineThickness,
                   position.y + f_textBoxOutlineThickness);
  rect.setOutlineThickness(f_textBoxOutlineThickness);
  sf::Text text{content, m_font};
  text.setCharacterSize(f_fontSize);
  text.setPosition(position.x + (width - text.getLocalBounds().width) * .5f,
                   position.y + f_textBoxTextVerticalPosition);
  switch (style) {
  case TextBoxStyle::Button:
    if (rect.getGlobalBounds().contains(
            m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)))) {
      highlighted = true;
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        rect.setFillColor(f_pressedButtonColor);
        rect.setOutlineColor(f_buttonOutlineColor);
        text.setFillColor(f_clickedButtonTextColor);
      } else {
        rect.setFillColor(f_barHighlightedButtonColor);
        rect.setOutlineColor(f_buttonOutlineColor);
        text.setFillColor(f_highlightedButtonTextColor);
      }
    } else {
      rect.setFillColor(f_barButtonColor);
      rect.setOutlineColor(f_buttonOutlineColor);
      text.setFillColor(f_buttonTextColor);
    }
    break;
  case TextBoxStyle::Text:
  default:
    rect.setFillColor(f_pressedButtonColor);
    rect.setOutlineColor(f_buttonOutlineColor);
    text.setFillColor(f_simpleTextBoxTextColor);
    break;
  }
  m_window.draw(rect);
  m_window.draw(text);
  return highlighted;
}

void View::drawCell(std::size_t col, std::size_t row) {
  auto rectSize{computeCellSize()};
  auto rect{sf::RectangleShape({rectSize.x - 2 * f_cellOutlineThickness,
                                rectSize.y - 2 * f_cellOutlineThickness})};
  auto rectPosition{computeCellPosition(col, row)};
  rect.setPosition(rectPosition.x + f_cellOutlineThickness,
                   rectPosition.y + f_cellOutlineThickness);
  rect.setOutlineColor(f_cellOutlineColor);
  rect.setOutlineThickness(f_cellOutlineThickness);
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
  if (drawTextBox("Quit [Esc]", position, maxCellSideLength * 3,
                  TextBoxStyle::Button)) {
    m_highlightedButton = Button::Quit;
  }
  position.x += maxCellSideLength * 3;
  if (drawTextBox("Restart", position, maxCellSideLength * 3,
                  TextBoxStyle::Button)) {
    m_highlightedButton = Button::Restart;
  }
  position.x += maxCellSideLength * 3;
  if (drawTextBox("9 x 9", position, maxCellSideLength * 3,
                  TextBoxStyle::Button)) {
    m_highlightedButton = Button::Size9x9;
  }
  position.x += maxCellSideLength * 3;
  if (drawTextBox("16 x 16", position, maxCellSideLength * 3,
                  TextBoxStyle::Button)) {
    m_highlightedButton = Button::Size16x16;
  }
  position.x += maxCellSideLength * 3;
  if (drawTextBox("30 x 16", position, maxCellSideLength * 3,
                  TextBoxStyle::Text)) {
    m_highlightedButton = Button::Size30x16;
  }
  position.x = f_defaultScreenWidth - 2 * maxCellSideLength * 3;
  if (drawTextBox("00:00", position, maxCellSideLength * 3,
                  TextBoxStyle::Text)) {
  }
  position.x += maxCellSideLength * 3;
  if (drawTextBox("99", position, maxCellSideLength * 3, TextBoxStyle::Text)) {
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
  auto menuBarSize{f_textBoxHeight};
  auto topLeftCellVPos{
      (static_cast<float>(windowSize.y) - menuBarSize - gridHeight) * 0.5f +
      menuBarSize};
  return {topLeftCellHPos + static_cast<float>(col) * cellSize.x,
          topLeftCellVPos + static_cast<float>(row) * cellSize.y};
}
