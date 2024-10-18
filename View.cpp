#include "View.hpp"
#include <SFML/Window/Mouse.hpp>
#include <iomanip>
#include <sstream>

namespace {
constexpr auto f_fontPath{"../resources/futura.ttf"};
constexpr auto f_1IconPath{"../resources/1.png"};
constexpr auto f_2IconPath{"../resources/2.png"};
constexpr auto f_3IconPath{"../resources/3.png"};
constexpr auto f_4IconPath{"../resources/4.png"};
constexpr auto f_5IconPath{"../resources/5.png"};
constexpr auto f_6IconPath{"../resources/6.png"};
constexpr auto f_7IconPath{"../resources/7.png"};
constexpr auto f_8IconPath{"../resources/8.png"};
constexpr auto f_flagIconPath{"../resources/flag.png"};
constexpr auto f_mineIconPath{"../resources/mine.png"};
constexpr auto f_questionMarkIconPath{"../resources/questionMark.png"};
constexpr auto f_fontSize{18};
constexpr auto f_zoomMaxLevel{1.f};
constexpr auto f_zoomMinLevel{2.f};
constexpr auto f_zoomSensibility{0.1f};
constexpr auto f_defaultCellSideLen{64.f};
constexpr auto f_zoomDefaultLevel{f_zoomMinLevel};
constexpr auto f_menuButtonHeight{57.f};
constexpr auto f_menuButtonWidth{192.f};
constexpr auto f_menuButtonTextVPosition{16.f};
constexpr auto f_menuButtonOutlineThickness{1.f};
constexpr auto f_cellButtonOutlineThickness{1.f};
const auto f_fontColor{sf::Color::White};
const auto f_cellButtonColor{sf::Color{70, 70, 70}};
const auto f_cellPressedButtonColor{sf::Color{40, 40, 40}};
const auto f_cellhighlightedButtonColor{sf::Color{90, 90, 90}};
const auto f_menuButtonColor{sf::Color{90, 90, 90}};
const auto f_menuHighlightedButtonColor{sf::Color{110, 110, 110}};
const auto f_menuPressedButtonColor{sf::Color{30, 30, 30}};
const auto f_buttonOutlineColor{sf::Color::Transparent};
const auto f_backgroundColor{sf::Color{50, 50, 50}};

inline std::string formattedTime(size_t seconds) {
  std::stringstream ss;
  auto min{seconds / 60};
  auto sec{seconds % 60};
  ss << std::setw(2) << std::setfill('0') << min;
  ss << ":";
  ss << std::setw(2) << sec;
  return ss.str();
}
} // namespace

View::View(sf::RenderWindow &window, Model &model)
    : m_model{model}, m_window{window}, m_font{},
      m_icons{{ButtonIcon::Mine, {}},
              {ButtonIcon::Flag, {}},
              {ButtonIcon::QuestionMark, {}}},
      m_highlightedButton{}, m_highlightedCellPos{}, m_zoomLevel{
                                                         f_zoomDefaultLevel} {
  loadResources();
}

View::Button View::highlightedButton() const { return m_highlightedButton; }

std::optional<std::pair<size_t, size_t>> View::highlightedCellPos() const {
  return m_highlightedCellPos;
}

void View::update() {
  m_window.clear();
  m_highlightedButton = Button::None;
  m_highlightedCellPos.reset();
  drawBackground();
  drawMenu();
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

void View::loadResources() {
  m_font.loadFromFile(f_fontPath);
  m_icons[ButtonIcon::One].loadFromFile(f_1IconPath);
  m_icons[ButtonIcon::Two].loadFromFile(f_2IconPath);
  m_icons[ButtonIcon::Three].loadFromFile(f_3IconPath);
  m_icons[ButtonIcon::Four].loadFromFile(f_4IconPath);
  m_icons[ButtonIcon::Five].loadFromFile(f_5IconPath);
  m_icons[ButtonIcon::Six].loadFromFile(f_6IconPath);
  m_icons[ButtonIcon::Seven].loadFromFile(f_7IconPath);
  m_icons[ButtonIcon::Eight].loadFromFile(f_8IconPath);
  m_icons[ButtonIcon::Mine].loadFromFile(f_mineIconPath);
  m_icons[ButtonIcon::Flag].loadFromFile(f_flagIconPath);
  m_icons[ButtonIcon::QuestionMark].loadFromFile(f_questionMarkIconPath);
  for (auto &icon : m_icons) {
    icon.second.setSmooth(true);
  }
}

void View::drawBackground() {
  auto windowSize{m_window.getSize()};
  ButtonArea background{
      {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)}};
  background.setFillColor(f_backgroundColor);
  m_window.draw(background);
}

void View::drawCells() {
  for (decltype(m_model.height()) row = 0; row < m_model.height(); row++) {
    for (decltype(m_model.width()) col = 0; col < m_model.width(); col++) {
      drawCellButton(col, row);
    }
  }
}

void View::drawMenu() {
  sf::Vector2f pos{0, 0};
  drawMenuButton(pos, Button::Quit);
  pos.x += f_menuButtonWidth;
  drawMenuButton(pos, Button::Restart);
  pos.x += f_menuButtonWidth;
  drawMenuButton(pos, Button::Size9x9);
  pos.x += f_menuButtonWidth;
  drawMenuButton(pos, Button::Size16x16);
  pos.x += f_menuButtonWidth;
  drawMenuButton(pos, Button::Size30x16);
  pos.x += f_menuButtonWidth;
  pos.x = static_cast<float>(m_window.getSize().x) - f_menuButtonWidth;
  drawMenuDisplay(pos, formattedTime(m_model.timeInSeconds()));
  pos.x -= f_menuButtonWidth;
  drawMenuDisplay(pos, std::to_string(m_model.markedMinesCount()));
}

void View::drawCellButton(std::size_t col, std::size_t row) {
  auto pos{cellButtonPosition(col, row)};
  auto area{makeButtonArea(pos, ButtonType::Cell)};
  auto &cell{m_model.cells()[col][row]};
  auto status{cellButtonStatus(area, cell)};
  area.setFillColor(buttonColor(ButtonType::Cell, status));
  m_window.draw(area);
  drawIconOnButton(area, cellButtonIcon(cell));
}

void View::drawMenuButton(const sf::Vector2f &pos, Button button) {
  auto area{makeButtonArea(pos, ButtonType::Menu)};
  area.setFillColor(
      buttonColor(ButtonType::Menu, menuButtonStatus(area, button)));
  m_window.draw(area);
  drawTextOnButton(area, buttonContent(button));
}

void View::drawMenuDisplay(const sf::Vector2f &pos,
                           const std::string &content) {
  auto area{makeButtonArea(pos, ButtonType::Menu)};
  area.setFillColor(buttonColor(ButtonType::Menu, ButtonStatus::Pressed));
  m_window.draw(area);
  drawTextOnButton(area, content);
}

void View::drawIconOnButton(ButtonArea &area, ButtonIcon icon) {
  if (icon == ButtonIcon::None) {
    return;
  }
  auto mask = area;
  mask.setFillColor(sf::Color::White);
  mask.setTexture(&m_icons.at(icon));
  m_window.draw(mask);
}

// See: https://en.sfml-dev.org/forums/index.php?topic=29521.0
void View::drawTextOnButton(ButtonArea &area, const std::string &content) {
  sf::Text text{content, m_font};
  text.setCharacterSize(f_fontSize);
  auto pos{area.getPosition()};
  auto buttonSize{area.getSize()};
  text.setPosition(pos.x + (buttonSize.x - text.getLocalBounds().width) * .5f,
                   pos.y + (buttonSize.y - text.getLocalBounds().height) * .5f);
  text.setFillColor(f_fontColor);
  m_window.draw(text);
}

View::ButtonArea View::makeButtonArea(const sf::Vector2f &pos,
                                      ButtonType type) const {
  auto size{buttonSize(type)};
  auto outlineThickness{buttonOutlineThickness(type)};
  ButtonArea rect{
      {size.x - 2 * outlineThickness, size.y - 2 * outlineThickness}};
  rect.setPosition(pos.x + outlineThickness, pos.y + outlineThickness);
  rect.setOutlineThickness(f_cellButtonOutlineThickness);
  rect.setOutlineColor(f_buttonOutlineColor);
  return rect;
}

float View::buttonOutlineThickness(ButtonType type) const {
  switch (type) {
  case ButtonType::Menu:
    return f_menuButtonOutlineThickness;
  default:
  case ButtonType::Cell:
    return f_cellButtonOutlineThickness;
  }
}

sf::Color View::buttonColor(ButtonType type, ButtonStatus status) const {
  switch (type) {
  case ButtonType::Menu:
    switch (status) {
    case ButtonStatus::Pressed:
      return f_menuPressedButtonColor;
    case ButtonStatus::Highlighted:
      return f_menuHighlightedButtonColor;
    default:
    case ButtonStatus::Released:
      return f_menuButtonColor;
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
}

sf::Vector2f View::buttonSize(ButtonType type) const {
  switch (type) {
  case ButtonType::Menu:
    return {f_menuButtonWidth, f_menuButtonHeight};
  default:
  case ButtonType::Cell:
    return cellButtonSize();
  }
}

sf::Vector2f View::cellButtonSize() const {
  auto sideLength{f_defaultCellSideLen * (m_zoomLevel / f_zoomMinLevel)};
  return {sideLength, sideLength};
}

sf::Vector2f View::cellButtonPosition(std::size_t col, std::size_t row) const {
  auto windowSize{m_window.getSize()};
  auto cellSize{cellButtonSize()};
  auto gridWidth{cellSize.x * static_cast<float>(m_model.width())};
  auto gridHeight{cellSize.y * static_cast<float>(m_model.height())};
  auto topLeftCellHPos{(static_cast<float>(windowSize.x) - gridWidth) * 0.5f};
  auto topLeftCellVPos{
      (static_cast<float>(windowSize.y) - f_menuButtonHeight - gridHeight) *
          0.5f +
      f_menuButtonHeight};
  return {topLeftCellHPos + static_cast<float>(col) * cellSize.x,
          topLeftCellVPos + static_cast<float>(row) * cellSize.y};
}

View::ButtonStatus View::menuButtonStatus(const ButtonArea &area,
                                          Button button) {
  auto status{buttonStatus(area)};
  if (status != ButtonStatus::Released) {
    m_highlightedButton = button;
  }
  switch (button) {
  case Button::Size9x9:
    if (m_model.size() == Model::Size::Size9x9) {
      status = ButtonStatus::Pressed;
    }
    break;
  case Button::Size16x16:
    if (m_model.size() == Model::Size::Size16x16) {
      status = ButtonStatus::Pressed;
    }
    break;
  case Button::Size30x16:
    if (m_model.size() == Model::Size::Size30x16) {
      status = ButtonStatus::Pressed;
    }
    break;
  case Button::None:
    status = ButtonStatus::Pressed;
  default:
    break;
  }
  return status;
}

View::ButtonStatus View::cellButtonStatus(const ButtonArea &area,
                                          const Cell &cell) {
  auto status{buttonStatus(area)};
  if (status != ButtonStatus::Released) {
    m_highlightedCellPos = {cell.col, cell.row};
  }
  switch (cell.status) {
  case Cell::Status::MarkedAsMine:
    status = ButtonStatus::Released;
    break;
  case Cell::Status::MarkedAsSuspect:
    status = ButtonStatus::Released;
    break;
  case Cell::Status::Revealed:
    status = ButtonStatus::Pressed;
    break;
  default:
    break;
  }
  return status;
}

View::ButtonStatus View::buttonStatus(const ButtonArea &area) const {
  auto isMouseHoveringButton{area.getGlobalBounds().contains(
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

std::string View::buttonContent(View::Button button) const {
  switch (button) {
  case View::Button::Quit:
    return "Quit";
  case View::Button::Restart:
    return "Restart";
    break;
  case View::Button::Size9x9:
    return "9x9";
    break;
  case View::Button::Size16x16:
    return "16";
  case View::Button::Size30x16:
    return "30x16";
  default:
    return "";
  }
}

View::ButtonIcon View::cellButtonIcon(const Cell &cell) const {
  switch (cell.status) {
  case Cell::Status::MarkedAsMine:
    return ButtonIcon::Flag;
  case Cell::Status::MarkedAsSuspect:
    return ButtonIcon::QuestionMark;
  case Cell::Status::Revealed:
    if (cell.type == Cell::Type::Mine) {
      return ButtonIcon::Mine;
    }
    switch (cell.neighbourMinesCount) {
    case 1:
      return ButtonIcon::One;
    case 2:
      return ButtonIcon::Two;
    case 3:
      return ButtonIcon::Three;
    case 4:
      return ButtonIcon::Four;
    case 5:
      return ButtonIcon::Five;
    case 6:
      return ButtonIcon::Six;
    case 7:
      return ButtonIcon::Seven;
    case 8:
      return ButtonIcon::Eight;
    default:
      return ButtonIcon::None;
    }
  default:
    return ButtonIcon::None;
  }
}
