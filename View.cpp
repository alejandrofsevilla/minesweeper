#include "View.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Window/Mouse.hpp>
#include <iomanip>
#include <sstream>

namespace {
constexpr auto f_fontPath{"../resources/futura.ttf"};
constexpr auto f_cellIconPath{"../resources/cell.png"};
constexpr auto f_buttonIconPath{"../resources/button.png"};
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
constexpr auto f_happyIconPath{"../resources/happy.png"};
constexpr auto f_sadIconPath{"../resources/sad.png"};
constexpr auto f_quitIconPath{"../resources/quit.png"};
constexpr auto f_fontSize{25};
constexpr auto f_zoomMaxLevel{1.f};
constexpr auto f_zoomMinLevel{2.f};
constexpr auto f_zoomSensibility{0.1f};
constexpr auto f_buttonSmallWidth{64.f};
constexpr auto f_buttonBigWidth{192.f};
constexpr auto f_buttonHeight{64.f};
constexpr auto f_zoomDefaultLevel{f_zoomMinLevel};
constexpr auto f_menuFrameHeight{64.f};
constexpr auto f_menuButtonTextVPosition{15.f};
constexpr auto f_menuButtonOutlineThickness{2.f};
constexpr auto f_menuFrameOutlineThickness{2.f};
constexpr auto f_cellButtonOutlineThickness{1.f};
constexpr auto f_iconSize{.85f};
const auto f_fontColor{sf::Color::White};
const auto f_cellButtonColor{sf::Color{120, 128, 136}};
const auto f_cellPressedButtonColor{sf::Color{56, 64, 72}};
const auto f_cellhighlightedButtonColor{sf::Color{200, 200, 200}};
const auto f_cellMineTriggeredColor{sf::Color{139, 0, 0}};
const auto f_cellFalseFlagColor{sf::Color{136, 51, 51}};
const auto f_menuFrameColor{sf::Color{68, 76, 84}};
const auto f_menuButtonColor{sf::Color{120, 128, 136}};
const auto f_menuHighlightedButtonColor{sf::Color{200, 200, 200}};
const auto f_menuPressedButtonColor{sf::Color{30, 30, 30}};
const auto f_buttonOutlineColor{sf::Color::Transparent};
const auto f_backgroundColor{sf::Color{30, 30, 30}};

inline std::string formattedTime(int seconds) {
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
      m_buttonUnderMouse{}, m_cellUnderMouse{}, m_zoomLevel{
                                                    f_zoomDefaultLevel} {
  loadResources();
}

View::Button View::buttonUnderMouse() const { return m_buttonUnderMouse; }

std::optional<std::pair<int, int>> View::cellUnderMouse() const {
  return m_cellUnderMouse;
}

void View::update() {
  m_window.clear();
  m_buttonUnderMouse = Button::None;
  m_cellUnderMouse.reset();
  drawBackground();
  drawCells();
  drawMenu();
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
  m_icons[ButtonIcon::SmallButton].loadFromFile(f_cellIconPath);
  m_icons[ButtonIcon::BigButton].loadFromFile(f_buttonIconPath);
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
  m_icons[ButtonIcon::Happy].loadFromFile(f_happyIconPath);
  m_icons[ButtonIcon::Sad].loadFromFile(f_sadIconPath);
  m_icons[ButtonIcon::Quit].loadFromFile(f_quitIconPath);
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
  auto windowSize{m_window.getSize()};
  ButtonArea frame{{static_cast<float>(windowSize.x), f_menuFrameHeight}};
  frame.setFillColor(f_menuFrameColor);
  frame.setOutlineColor(f_backgroundColor);
  frame.setOutlineThickness(f_menuButtonOutlineThickness);
  m_window.draw(frame);
  sf::Vector2f pos{0, 0};
  drawMenuButton(pos, Button::Quit);
  pos.x += f_buttonBigWidth;
  drawMenuButton(pos, Button::Size);
  pos.x += f_buttonBigWidth;
  drawMenuButton(pos, Button::Restart);
  pos.x = static_cast<float>(m_window.getSize().x) - f_buttonBigWidth;
  drawMenuDisplay(pos, formattedTime(m_model.timeInSeconds()));
  pos.x -= f_buttonBigWidth;
  drawMenuDisplay(pos, std::to_string(m_model.minesCount()));
}

void View::drawCellButton(int col, int row) {
  auto pos{cellButtonPosition(col, row)};
  auto area{makeButtonArea(pos, ButtonType::Small)};
  auto &cell{m_model.cells()[col][row]};
  auto status{cellButtonStatus(area, cell)};
  if (status != ButtonStatus::Pressed) {
    area.setTexture(&m_icons.at(ButtonIcon::SmallButton));
  }
  area.setFillColor(buttonColor(ButtonType::Small, status));
  if (cell.triggered) {
    area.setFillColor(f_cellMineTriggeredColor);
  } else if (cell.status == Cell::Status::MarkedAsMine &&
             cell.type != Cell::Type::Mine &&
             m_model.status() == Model::Status::Finished) {
    area.setFillColor(f_cellFalseFlagColor);
  }
  m_window.draw(area);
  drawIconOnButton(area, cellButtonIcon(cell));
}

void View::drawMenuButton(const sf::Vector2f &pos, Button button) {
  auto area{makeButtonArea(pos, ButtonType::Big)};
  auto status{menuButtonStatus(area, button)};
  area.setFillColor(buttonColor(ButtonType::Big, status));
  if (status != ButtonStatus::Pressed) {
    area.setTexture(&m_icons.at(ButtonIcon::BigButton));
  }
  m_window.draw(area);
  switch (button) {
  case Button::Quit:
    return drawIconOnButton(area, ButtonIcon::Quit);
  case Button::Restart:
    if (m_model.status() == Model::Status::Finished && !m_model.success()) {
      return drawIconOnButton(area, ButtonIcon::Sad);
    }
    return drawIconOnButton(area, ButtonIcon::Happy);
  default:
    return drawTextOnButton(area, buttonContent(button));
  }
}

void View::drawMenuDisplay(const sf::Vector2f &pos,
                           const std::string &content) {
  auto area{makeButtonArea(pos, ButtonType::Big)};
  area.setFillColor(buttonColor(ButtonType::Big, ButtonStatus::Pressed));
  m_window.draw(area);
  drawTextOnButton(area, content);
}

void View::drawIconOnButton(ButtonArea &area, ButtonIcon icon) {
  if (icon == ButtonIcon::None) {
    return;
  }
  auto mask = area;
  auto areaSize{area.getSize()};
  auto maskSize{areaSize * f_iconSize};
  mask.setSize(maskSize);
  auto pos{mask.getPosition()};
  mask.setPosition(pos.x + (areaSize.x - maskSize.x) * .5f,
                   pos.y + (areaSize.y - maskSize.y) * .5f);
  mask.setFillColor(sf::Color::White);
  mask.setTexture(&m_icons.at(icon));
  m_window.draw(mask);
}

// See: https://en.sfml-dev.org/forums/index.php?topic=29521.0
void View::drawTextOnButton(ButtonArea &area, const std::string &content) {
  sf::Text text{content, m_font};
  text.setCharacterSize(f_fontSize);
  text.setStyle(sf::Text::Bold);
  auto pos{area.getPosition()};
  auto size{area.getSize()};
  text.setPosition(pos.x + (size.x - text.getLocalBounds().width) * .5f,
                   pos.y + f_menuButtonTextVPosition);
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
  case ButtonType::Big:
    return f_menuButtonOutlineThickness;
  default:
  case ButtonType::Small:
    return f_cellButtonOutlineThickness;
  }
}

sf::Color View::buttonColor(ButtonType type, ButtonStatus status) const {
  switch (type) {
  case ButtonType::Big:
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
  case ButtonType::Small:
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
  case ButtonType::Big:
    return {f_buttonBigWidth, f_buttonHeight};
  default:
  case ButtonType::Small:
    return cellButtonSize();
  }
}

sf::Vector2f View::cellButtonSize() const {
  auto sideLength{f_buttonSmallWidth * (m_zoomLevel / f_zoomMinLevel)};
  return {sideLength, sideLength};
}

sf::Vector2f View::cellButtonPosition(int col, int row) const {
  auto windowSize{m_window.getSize()};
  auto cellSize{cellButtonSize()};
  auto gridWidth{cellSize.x * static_cast<float>(m_model.width())};
  auto gridHeight{cellSize.y * static_cast<float>(m_model.height())};
  auto topLeftCellHPos{(static_cast<float>(windowSize.x) - gridWidth) * 0.5f};
  auto topLeftCellVPos{
      (static_cast<float>(windowSize.y) - f_buttonHeight - gridHeight) * 0.5f +
      f_buttonHeight};
  return {topLeftCellHPos + static_cast<float>(col) * cellSize.x,
          topLeftCellVPos + static_cast<float>(row) * cellSize.y};
}

View::ButtonStatus View::menuButtonStatus(const ButtonArea &area,
                                          Button button) {
  auto status{buttonStatus(area)};
  if (status != ButtonStatus::Released) {
    m_buttonUnderMouse = button;
  }
  switch (button) {
  case Button::Size:
    if (m_model.status() != Model::Status::Ready) {
      status = ButtonStatus::Released;
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
    m_cellUnderMouse = {cell.col, cell.row};
  }
  if (m_model.status() == Model::Status::Finished) {
    status = ButtonStatus::Released;
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
  case View::Button::Size:
    switch (m_model.size()) {
    case Model::Size::Size9x9:
      return "9x9";
    case Model::Size::Size16x16:
      return "16x16";
    default:
    case Model::Size::Size30x16:
      return "30x16";
    }
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
