#include "Controller.hpp"
#include "Model.hpp"
#include "View.hpp"

namespace {
constexpr auto f_windowTitle{"Minesweeper"};
constexpr auto f_windowStyle{sf::Style::Fullscreen};
constexpr auto f_antialiasing{4};
} // namespace

int main() {
  sf::RenderWindow window{sf::VideoMode::getDesktopMode(), f_windowTitle,
                          f_windowStyle,
                          sf::ContextSettings{0, 0, f_antialiasing}};
  window.setVerticalSyncEnabled(true);
  Model model;
  View view{window, model};
  Controller controller{view, model};
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      controller.onEvent(event);
    }
    model.update();
    view.update();
  }
  return 0;
}
