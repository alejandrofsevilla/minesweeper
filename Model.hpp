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
  int width() const;
  int height() const;
  int minesCount() const;
  int timeInSeconds() const;

  const std::vector<std::vector<Cell>> &cells() const;

  void update();
  void restart();
  void setSize(Size size);
  void cycleCellStatus(int col, int row);
  void reveal(int col, int row);
  void tryRevealNeighbours(int col, int row);

private:
  void startTime();
  void updateTime();
  void generateCells();
  void generateMines();
  void revealAllMines();
  int countNeighbourMines(int col, int row);

  Status m_status;
  Size m_size;
  int m_timeInSeconds;
  int m_minesCount;
  int m_markedMinesCount;
  std::vector<std::vector<Cell>> m_cells;
  std::chrono::system_clock::time_point m_startTime;
};

#endif
