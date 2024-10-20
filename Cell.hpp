#ifndef MINESWEEPER_CELL_HPP
#define MINESWEEPER_CELL_HPP

struct Cell {
  enum class Type { Empty, Mine };
  enum class Status { Hidden, Revealed, MarkedAsMine, MarkedAsSuspect };

  int col{0};
  int row{0};
  int neighbourMinesCount{0};
  Type type{Type::Empty};
  Status status{Status::Hidden};
  bool triggered{false};
};

#endif
