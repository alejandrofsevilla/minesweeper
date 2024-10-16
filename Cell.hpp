#ifndef MINESWEEPER_CELL_HPP
#define MINESWEEPER_CELL_HPP

struct Cell {
  enum class Type { Empty, Mine };
  enum class Status {
    Hidden,
    Revealed,
    Flagged,
    Suspected,
    /*TODO: remove -> */ Alive,
    /*TODO: remove -> */ Dead,
    /*TODO: remove -> */ Empty
  };

  std::size_t col{0};
  std::size_t row{0};
  Type type{Type::Empty};
  Status status{Status::Hidden};

  bool operator==(const Cell &other) const {
    return col == other.col && row == other.row;
  }
  bool operator<(const Cell &other) const {
    if (row < other.row) {
      return true;
    }
    if (row > other.row) {
      return false;
    }
    if (col < other.col) {
      return true;
    }
    return false;
  }
  bool operator>(const Cell &other) const {
    return !(*this == other) && !(*this < other);
  }
};

#endif
