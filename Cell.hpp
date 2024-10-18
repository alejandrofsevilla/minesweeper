#ifndef MINESWEEPER_CELL_HPP
#define MINESWEEPER_CELL_HPP

struct Cell {
  enum class Type { Empty, Mine };
  enum class Status { Hidden, Revealed, MarkedAsMine, MarkedAsSuspect };

  size_t col{0};
  size_t row{0};
  size_t neighbourMinesCount{0};
  Type type{Type::Empty};
  Status status{Status::Hidden};
};

#endif
