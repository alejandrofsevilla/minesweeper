#include "View.hpp"
#include <SFML/Window/Mouse.hpp>
#include <iomanip>
#include <sstream>

namespace {
constexpr auto f_fontPath{"../resources/futura.ttf"};
constexpr auto f_smallButtonIconPath{"../resources/smallButton.png"};
constexpr auto f_mediumButtonIconPath{"../resources/mediumButton.png"};
constexpr auto f_bigButtonIconPath{"../resources/bigButton.png"};
constexpr auto f_button11MiddleIconPath{"../resources/button11Middle.png"};
constexpr auto f_button13MiddleIconPath{"../resources/button13Middle.png"};
constexpr auto f_button2MiddleIconPath{"../resources/button2Middle.png"};
constexpr auto f_button3LeftIconPath{"../resources/button3Left.png"};
constexpr auto f_button1RightIconPath{"../resources/button1Right.png"};
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
constexpr auto f_zoomDefaultLevel{f_zoomMinLevel};
constexpr auto f_buttonSmallWidth{63.5f};
constexpr auto f_buttonMediumWidth{2 * f_buttonSmallWidth};
constexpr auto f_buttonBigWidth{3 * f_buttonSmallWidth};
constexpr auto f_buttonHeight{63.5f};
constexpr auto f_iconSize{.85f};
constexpr auto f_defaultWindowWidth{1920};
constexpr auto f_defaultWindowHeight{1080};
constexpr auto f_menuFrameHeight{64.f};
constexpr auto f_menuButtonTextVOffset{5.f};
constexpr auto f_menuLeftMargin{
    (f_defaultWindowWidth - 30 * f_buttonSmallWidth) * .5f};
constexpr auto f_buttonOutlineThickness{1.f};
constexpr auto f_displayOutlineThickness{10.f};
const auto f_fontColor{sf::Color::White};
const auto f_buttonColor{sf::Color{120, 128, 136}};
const auto f_buttonPressedColor{sf::Color{56, 64, 72}};
const auto f_buttonHighlightedColor{sf::Color{200, 200, 200}};
const auto f_cellMineTriggeredColor{sf::Color{139, 0, 0}};
const auto f_cellFalseFlagColor{sf::Color{136, 51, 51}};
const auto f_menuFrameColor{sf::Color{68, 76, 84}};
const auto f_menuButtonColor{sf::Color{120, 128, 136}};
const auto f_menuHighlightedButtonColor{sf::Color{200, 200, 200}};
const auto f_menuDisplayColor{sf::Color::Black};
const auto f_buttonOutlineColor{sf::Color::Transparent};
const auto f_backgroundColor{sf::Color{40, 40, 40}};

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
      m_buttonUnderMouse{}, m_cellUnderMouse{},
      m_zoomLevel{f_zoomDefaultLevel} {
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
  scaleWindow();
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
  m_icons[ButtonIcon::ButtonStandard].loadFromFile(f_smallButtonIconPath);
  m_icons[ButtonIcon::Button11Middle].loadFromFile(f_button11MiddleIconPath);
  m_icons[ButtonIcon::Button13Middle].loadFromFile(f_button13MiddleIconPath);
  m_icons[ButtonIcon::Button2Middle].loadFromFile(f_button2MiddleIconPath);
  m_icons[ButtonIcon::Button1Right].loadFromFile(f_button1RightIconPath);
  m_icons[ButtonIcon::Button3Left].loadFromFile(f_button3LeftIconPath);
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
  ButtonArea background{{static_cast<float>(f_defaultWindowWidth),
                         static_cast<float>(f_defaultWindowHeight)}};
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
  auto frame{
      makeButtonArea({f_menuLeftMargin, 0.}, f_buttonOutlineThickness, 30)};
  frame.setFillColor(f_menuFrameColor);
  frame.setOutlineColor(f_backgroundColor);
  m_window.draw(frame);
  drawMenuButton(0, 3, Button::Size, ButtonType::Button3Left);
  drawMenuButton(3, 11, Button::None, ButtonType::Button11Middle);
  drawMenuDisplay(11, 3, std::to_string(m_model.minesCount()));
  drawMenuButton(14, 2, Button::Restart, ButtonType::Button2Middle);
  drawMenuButton(16, 13, Button::None, ButtonType::Button13Middle);
  drawMenuDisplay(16, 3, formattedTime(m_model.timeInSeconds()));
  drawMenuButton(29, 1, Button::Quit, ButtonType::Button1Right);
}

void View::drawCellButton(int col, int row) {
  auto pos{cellButtonPosition(col, row)};
  auto area{makeButtonArea(pos, 1, f_buttonOutlineThickness)};
  area.setSize(cellButtonSize() - sf::Vector2f{2.f * f_buttonOutlineThickness,
                                               2.f * f_buttonOutlineThickness});
  auto &cell{m_model.cells()[col][row]};
  auto status{cellButtonStatus(area, cell)};
  if (status != ButtonStatus::Pressed) {
    area.setTexture(&m_icons.at(ButtonIcon::ButtonStandard));
  }
  area.setFillColor(buttonColor(status));
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

void View::drawMenuButton(int col, int width, Button button, ButtonType type) {
  auto area{makeButtonArea({f_menuLeftMargin + col * f_buttonSmallWidth, 0},
                           width, f_buttonOutlineThickness)};
  auto status{menuButtonStatus(area, button)};
  area.setFillColor(buttonColor(status));
  if (status != ButtonStatus::Pressed) {
    area.setTexture(&m_icons.at(menuButtonIcon(type)));
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

void View::drawMenuDisplay(int col, int width, const std::string &content) {
  auto area{makeButtonArea({f_menuLeftMargin + col * f_buttonSmallWidth, 0},
                           width, f_displayOutlineThickness)};
  area.setFillColor(f_menuDisplayColor);
  m_window.draw(area);
  drawTextOnButton(area, content);
}

void View::drawIconOnButton(ButtonArea &area, ButtonIcon icon) {
  if (icon == ButtonIcon::None) {
    return;
  }
  auto &mask = area;
  auto areaSize{area.getSize()};
  auto maskSize{areaSize * f_iconSize};
  mask.setSize(maskSize);
  auto &pos{mask.getPosition()};
  mask.setPosition(pos.x + (areaSize.x - maskSize.x) * .5f,
                   pos.y + (areaSize.y - maskSize.y) * .5f);
  mask.setFillColor(sf::Color::White);
  mask.setTexture(&m_icons.at(icon));
  m_window.draw(mask);
}

void View::drawTextOnButton(ButtonArea &area, const std::string &content) {
  sf::Text text{content, m_font};
  text.setCharacterSize(f_fontSize);
  text.setStyle(sf::Text::Bold);
  auto &pos{area.getPosition()};
  auto &size{area.getSize()};
  text.setPosition(pos.x + (size.x - text.getLocalBounds().width) * .5f,
                   pos.y + (size.y - text.getLocalBounds().height) * .5f -
                       +f_menuButtonTextVOffset);
  text.setFillColor(f_fontColor);
  m_window.draw(text);
}

void View::scaleWindow() {
  auto view{m_window.getView()};
  view.setSize(f_defaultWindowWidth, f_defaultWindowHeight);
  view.setCenter(f_defaultWindowWidth * .5f, f_defaultWindowHeight * .5f);
  m_window.setView(view);
}

View::ButtonArea View::makeButtonArea(const sf::Vector2f &pos, int width,
                                      float outlineThickness) const {
  ButtonArea rect{{width * f_buttonSmallWidth - 2 * outlineThickness,
                   f_buttonHeight - 2 * outlineThickness}};
  rect.setPosition(pos.x + outlineThickness, pos.y + outlineThickness);
  rect.setOutlineThickness(outlineThickness);
  rect.setOutlineColor(f_buttonOutlineColor);
  return rect;
}

sf::Color View::buttonColor(ButtonStatus status) const {
  switch (status) {
  case ButtonStatus::Pressed:
    return f_buttonPressedColor;
  case ButtonStatus::Highlighted:
    return f_buttonHighlightedColor;
  default:
  case ButtonStatus::Released:
    return f_buttonColor;
  }
}

sf::Vector2f View::cellButtonSize() const {
  auto width{f_buttonSmallWidth * (m_zoomLevel / f_zoomMinLevel)};
  auto height{f_buttonHeight * (m_zoomLevel / f_zoomMinLevel)};
  return {width, height};
}

sf::Vector2f View::cellButtonPosition(int col, int row) const {
  auto cellSize{cellButtonSize()};
  auto gridWidth{cellSize.x * static_cast<float>(m_model.width())};
  auto gridHeight{cellSize.y * static_cast<float>(m_model.height())};
  auto topLeftCellHPos{(static_cast<float>(f_defaultWindowWidth) - gridWidth) *
                       0.5f};
  auto topLeftCellVPos{(static_cast<float>(f_defaultWindowHeight) -
                        f_buttonHeight - gridHeight) *
                           0.5f +
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
    status = ButtonStatus::Released;
    break;
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

View::ButtonIcon View::menuButtonIcon(ButtonType type) const {
  switch (type) {
  case ButtonType::Button11Middle:
    return ButtonIcon::Button11Middle;
  case ButtonType::Button13Middle:
    return ButtonIcon::Button13Middle;
  case ButtonType::Button1Right:
    return ButtonIcon::Button1Right;
  case ButtonType::Button3Left:
    return ButtonIcon::Button3Left;
  case ButtonType::Button2Middle:
    return ButtonIcon::Button2Middle;
  default:
  case ButtonType::Standard:
    return ButtonIcon::ButtonStandard;
  }
}