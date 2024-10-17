#include "View.hpp"
#include <SFML/Window/Mouse.hpp>

namespace {
constexpr auto f_fontPath{"../resources/futura.ttf"};
constexpr auto f_fontSize{18};
constexpr auto f_zoomMaxLevel{1.f};
constexpr auto f_zoomMinLevel{2.f};
constexpr auto f_zoomSensibility{0.1f};
constexpr auto f_zoomDefaultLevel{f_zoomMinLevel};
constexpr auto f_menuBarButtonHeight{57.f};
constexpr auto f_menuBarButtonWidth{192.f};
constexpr auto f_menuBarButtonTextVPosition{16.f};
constexpr auto f_menuBarButtonOutlineThickness{2.f};
constexpr auto f_cellButtonOutlineThickness{1.f};
const auto f_fontColor{sf::Color::White};
const auto f_cellButtonColor{sf::Color{70, 70, 70}};
const auto f_cellPressedButtonColor{sf::Color{40, 40, 40}};
const auto f_cellhighlightedButtonColor{sf::Color{90, 90, 90}};
const auto f_menuBarButtonColor{sf::Color{90, 90, 90}};
const auto f_menuBarHighlightedButtonColor{sf::Color{110, 110, 110}};
const auto f_menuBarPressedButtonColor{sf::Color{30, 30, 30}};
const auto f_buttonOutlineColor{sf::Color::Transparent};
const auto f_backgroundColor{sf::Color{50, 50, 50}};
} // namespace

View::View(sf::RenderWindow &window, Model &model)
    : m_model{model}, m_window{window}, m_font{}, m_highlightedButton{},
      m_highlightedCell{}, m_zoomLevel{f_zoomDefaultLevel} {
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
  drawMenuBar();
  drawCells();
  m_window.display();
}

void View::zoomIn() {
  m_zoomLevel = std::min(m_zoomLevel + f_zoomSensibility, f_zoomMinLevel);
}

void View::zoomOut() {
  m_zoomLevel = std::max(m_zoomLevel - f_zoomSensibility, f_zoomMaxLevel);
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
      drawCellButton(col, row); // TODO: Model has simply a vector of cells
    }
  }
}

void View::drawMenuBar() {
  sf::Vector2f position{0, 0};
  drawMenuBarButton(position, Button::Quit, ButtonStatus::Released);
  position.x += f_menuBarButtonWidth;
  drawMenuBarButton(position, Button::Restart, ButtonStatus::Released);
  position.x += f_menuBarButtonWidth;
  auto modelSize{m_model.size()};
  drawMenuBarButton(position, Button::Size9x9,
                    modelSize == Model::Size::Size9x9 ? ButtonStatus::Pressed
                                                      : ButtonStatus::Released);
  position.x += f_menuBarButtonWidth;
  drawMenuBarButton(position, Button::Size16x16,
                    modelSize == Model::Size::Size9x9 ? ButtonStatus::Pressed
                                                      : ButtonStatus::Released);
  position.x += f_menuBarButtonWidth;
  drawMenuBarButton(position, Button::Size30x16,
                    modelSize == Model::Size::Size30x16
                        ? ButtonStatus::Pressed
                        : ButtonStatus::Released);
  position.x += f_menuBarButtonWidth;
  position.x = static_cast<float>(m_window.getSize().x) - f_menuBarButtonWidth;
  auto timeDisplay{makeButton(position, ButtonType::MenuBar)};
  timeDisplay.setFillColor(
      computeButtonColor(ButtonType::MenuBar, ButtonStatus::Pressed));
  fillAndDrawButton(timeDisplay, "00:00");

  position.x -= f_menuBarButtonWidth;
  auto minesDisplay{makeButton(position, ButtonType::MenuBar)};
  minesDisplay.setFillColor(
      computeButtonColor(ButtonType::MenuBar, ButtonStatus::Pressed));
  fillAndDrawButton(minesDisplay, "99");
}

void View::drawCellButton(std::size_t col, std::size_t row) {
  auto position{computeCellPosition(col, row)};
  auto button{makeButton(position, ButtonType::Cell)};
  auto buttonStatus{computeButtonStatus(button)};
  if (buttonStatus != ButtonStatus::Released) {
    m_highlightedCell = {col, row};
  }
  switch (m_model.cells()[col][row].status) {
    // TODO: icons
  case Cell::Status::Hidden:
    break;
  case Cell::Status::Flagged:
    break;
  case Cell::Status::Suspected:
    break;
  case Cell::Status::Revealed:
    buttonStatus = ButtonStatus::Pressed;
    break;
  default:
    break;
  }
  button.setFillColor(computeButtonColor(ButtonType::Cell, buttonStatus));
  fillAndDrawButton(button, ""); // TODO: draw with icon instead
  m_window.draw(button);
}

void View::drawMenuBarButton(const sf::Vector2f &position, Button buttonId,
                             ButtonStatus status) {
  auto button{makeButton(position, ButtonType::MenuBar)};
  if (status != ButtonStatus::Pressed) {
    status = computeButtonStatus(button);
    if (status != ButtonStatus::Released) {
      m_highlightedButton = buttonId;
    }
  }
  button.setFillColor(computeButtonColor(ButtonType::MenuBar, status));
  switch (buttonId) {
  case Button::Quit:
    fillAndDrawButton(button, "Quit");
    break;
  case Button::Restart:
    fillAndDrawButton(button, "Restart");
    break;
  case Button::Size9x9:
    fillAndDrawButton(button, "9x9");
    break;
  case Button::Size16x16:
    fillAndDrawButton(button, "16x16");
    break;
  case Button::Size30x16:
    fillAndDrawButton(button, "30x16");
    break;
  }
}

void View::fillAndDrawButton(sf::RectangleShape &button,
                             const sf::Texture &icon) {
  button.setTexture(&icon);
  m_window.draw(button);
}

void View::fillAndDrawButton(sf::RectangleShape &button,
                             const std::string &content) {
  sf::Text text{content, m_font};
  text.setCharacterSize(f_fontSize);
  auto buttonPosition{button.getPosition()};
  auto buttonSize{button.getSize()};
  text.setPosition(buttonPosition.x +
                       (buttonSize.x - text.getLocalBounds().width) * .5f,
                   buttonPosition.y + f_menuBarButtonTextVPosition);
  text.setFillColor(f_fontColor);
  m_window.draw(button);
  m_window.draw(text);
}

sf::RectangleShape View::makeButton(const sf::Vector2f &position,
                                    ButtonType type) {
  auto size{computeButtonSize(type)};
  auto outlineThickness{computeButtonOutlineThickness(type)};
  sf::RectangleShape rect{
      {size.x - 2 * outlineThickness, size.y - 2 * outlineThickness}};
  rect.setPosition(position.x + outlineThickness,
                   position.y + outlineThickness);
  rect.setOutlineThickness(f_cellButtonOutlineThickness);
  rect.setOutlineColor(f_buttonOutlineColor);
  return rect;
}

float View::computeButtonOutlineThickness(ButtonType type) const {
  switch (type) {
  case ButtonType::MenuBar:
    return f_menuBarButtonOutlineThickness;
  default:
  case ButtonType::Cell:
    return f_cellButtonOutlineThickness;
  }
}

sf::Color View::computeButtonColor(ButtonType type, ButtonStatus status) const {
  switch (type) {
  case ButtonType::MenuBar:
    switch (status) {
    case ButtonStatus::Pressed:
      return f_menuBarPressedButtonColor;
    case ButtonStatus::Highlighted:
      return f_menuBarHighlightedButtonColor;
    default:
    case ButtonStatus::Released:
      return f_menuBarButtonColor;
    }
  default:
  case ButtonType::Cell:
    switch (status) {
    case ButtonStatus::Pressed:
      return f_cellPressedButtonColor;
    case ButtonStatus::Highlighted:
      return f_cellhighlightedButtonColor;
    default:
    case ButtonStatus::Released:
      return f_cellButtonColor;
    }
  }
  return {};
}

sf::Vector2f View::computeButtonSize(ButtonType type) const {
  switch (type) {
  case ButtonType::MenuBar:
    return {f_menuBarButtonWidth, f_menuBarButtonHeight};
  default:
  case ButtonType::Cell:
    return computeCellSize();
  }
  return {};
}

sf::Vector2f View::computeCellSize() const {
  auto windowSize{m_window.getSize()};
  auto maxSideLength{static_cast<float>(windowSize.x) /
                     static_cast<float>(m_model.width())};
  auto sideLength{maxSideLength * (m_zoomLevel / f_zoomMinLevel)};
  return {sideLength, sideLength};
}

sf::Vector2f View::computeCellPosition(std::size_t col, std::size_t row) const {
  auto windowSize{m_window.getSize()};
  auto cellSize{computeCellSize()};
  auto gridWidth{cellSize.x * static_cast<float>(m_model.width())};
  auto gridHeight{cellSize.y * static_cast<float>(m_model.height())};
  auto topLeftCellHPos{(static_cast<float>(windowSize.x) - gridWidth) * 0.5f};
  auto menuBarSize{f_menuBarButtonHeight};
  auto topLeftCellVPos{
      (static_cast<float>(windowSize.y) - menuBarSize - gridHeight) * 0.5f +
      menuBarSize};
  return {topLeftCellHPos + static_cast<float>(col) * cellSize.x,
          topLeftCellVPos + static_cast<float>(row) * cellSize.y};
}

View::ButtonStatus
View::computeButtonStatus(const sf::RectangleShape &button) const {
  auto isMouseHoveringButton{button.getGlobalBounds().contains(
      m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)))};
  auto isMousePressed{sf::Mouse::isButtonPressed(sf::Mouse::Left)};
  if (isMouseHoveringButton) {
    if (isMousePressed) {
      return ButtonStatus::Pressed;
    }
    return ButtonStatus::Highlighted;
  }
  return ButtonStatus::Released;
};
