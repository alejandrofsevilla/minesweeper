#ifndef MINESWEEPER_MODEL_HPP
#define MINESWEEPER_MODEL_HPP

#include <chrono>
#include <vector>

#include "Cell.hpp"

class Model {
public:
  enum class Status { Ready, Started, Running, Stopped, Finished };
  enum class Size { Size9x9, Size16x16, Size30x16 };

  Model();

  Size size() const;
  Status status() const;
  size_t width() const;
  size_t height() const;
  size_t markedMinesCount() const;
  size_t timeInSeconds() const;

  const std::vector<std::vector<Cell>> &cells() const;

  void update();
  void restart();
  void setSize(Size size);
  void cycleCellStatus(std::size_t col, std::size_t row);
  void reveal(std::size_t col, std::size_t row);

private:
  void startTime();
  void updateTime();
  void generateCells();
  void generateMines();
  void revealAllMines();
  void revealNeighbours(std::size_t col, std::size_t row);

  Status m_status;
  Size m_size;
  size_t m_timeInSeconds;
  size_t m_markedMinesCount;
  std::vector<std::vector<Cell>> m_cells;
  std::chrono::system_clock::time_point m_startTime;
};

#endif
