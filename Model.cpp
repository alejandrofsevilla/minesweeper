#include "Model.hpp"

#include <random>

namespace {
inline int generateRandomValue(int min, int max) {
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<int> distr{min, max};
  return distr(gen);
}

inline int numberOfMines(Model::Size size) {
  switch (size) {
  case Model::Size::Size9x9:
    return 10;
  case Model::Size::Size16x16:
    return 40;
  default:
  case Model::Size::Size30x16:
    return 99;
  }
}

inline std::pair<int, int> sizeAsPair(Model::Size size) {
  switch (size) {
  case Model::Size::Size9x9:
    return {9, 9};
  case Model::Size::Size16x16:
    return {16, 16};
  default:
  case Model::Size::Size30x16:
    return {30, 16};
  }
}
} // namespace

Model::Model()
    : m_status{Status::Ready}, m_size{Size::Size30x16}, m_timeInSeconds{0},
      m_minesCount{0}, m_markedMinesCount{0}, m_cells{}, m_startTime{} {}

Model::Size Model::size() const { return m_size; }

Model::Status Model::status() const { return m_status; }

int Model::width() const { return sizeAsPair(m_size).first; }

int Model::height() const { return sizeAsPair(m_size).second; }

int Model::minesCount() const { return m_minesCount - m_markedMinesCount; };

int Model::timeInSeconds() const { return m_timeInSeconds; }

const std::vector<std::vector<Cell>> &Model::cells() const { return m_cells; }

void Model::update() {
  switch (m_status) {
  case Status::Ready:
    if (m_cells.empty()) {
      restart();
    }
    return;
  case Status::Started:
    m_startTime = std::chrono::system_clock::now();
    m_status = Status::Running;
    return;
  case Status::Running:
    updateTime();
    return;
  case Status::Stopped:
    revealAllMines();
    m_status = Status::Finished;
    return;
  case Status::Finished:
    return;
  }
}

void Model::cycleCellStatus(int col, int row) {
  auto &cell{m_cells[col][row]};
  switch (cell.status) {
  case Cell::Status::Hidden:
    cell.status = Cell::Status::MarkedAsMine;
    m_markedMinesCount++;
    return;
  case Cell::Status::MarkedAsMine:
    m_markedMinesCount--;
    cell.status = Cell::Status::MarkedAsSuspect;
    return;
  case Cell::Status::MarkedAsSuspect:
    cell.status = Cell::Status::Hidden;
    return;
  default:
    return;
  }
}

void Model::reveal(int col, int row) {
  auto &cell = m_cells[col][row];
  if (cell.status != Cell::Status::Hidden) {
    return;
  }
  cell.status = Cell::Status::Revealed;
  if (cell.type == Cell::Type::Mine) {
    m_status = Status::Stopped;
    return;
  }
  tryRevealNeighbours(col, row);
  if (m_status == Status::Ready) {
    m_status = Status::Started;
  }
}

void Model::tryRevealNeighbours(int col, int row) {
  auto &cell{m_cells[col][row]};
  if (cell.status != Cell::Status::Revealed) {
    return;
  }
  auto gridSize{sizeAsPair(m_size)};
  auto width{gridSize.first};
  auto height{gridSize.second};
  std::vector<Cell *> neighbours;
  decltype(Cell::neighbourMinesCount) neighbourMarkedMinesCount{0};
  for (auto i = col - 1; i <= col + 1; i++) {
    for (auto j = row - 1; j <= row + 1; j++) {
      if ((i >= 0 && i < width) && (j >= 0 && j < height)) {
        auto &neighbour{m_cells[i][j]};
        if (neighbour.status == Cell::Status::MarkedAsMine) {
          neighbourMarkedMinesCount++;
        }
        neighbours.push_back(&neighbour);
      }
    }
  }
  if (neighbourMarkedMinesCount != cell.neighbourMinesCount) {
    return;
  }
  for (auto &n : neighbours) {
    reveal(n->col, n->row);
  }
}

void Model::setSize(Size size) {
  if (size == m_size) {
    return;
  }
  m_size = size;
  restart();
}

void Model::restart() {
  m_cells.clear();
  m_minesCount = numberOfMines(m_size);
  m_markedMinesCount = 0;
  m_timeInSeconds = 0;
  generateCells();
  generateMines();
  m_status = Status::Ready;
}

void Model::updateTime() {
  m_timeInSeconds =
      static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(
                           std::chrono::system_clock::now() - m_startTime)
                           .count());
}

void Model::generateCells() {
  auto gridSize{sizeAsPair(m_size)};
  auto width{gridSize.first};
  auto height{gridSize.second};
  for (auto col = 0; col < width; col++) {
    m_cells.push_back({});
    for (auto row = 0; row < height; row++) {
      m_cells[col].push_back(
          {col, row, 0, Cell::Type::Empty, Cell::Status::Hidden});
    }
  }
}

void Model::generateMines() {
  auto gridSize{sizeAsPair(m_size)};
  auto width{gridSize.first};
  auto height{gridSize.second};
  for (auto i = 0; i < m_minesCount; i++) {
    auto pos{generateRandomValue(0, width * height - 1)};
    auto col = pos % width;
    auto row = pos / width;
    m_cells[col][row].type = Cell::Type::Mine;
  }
  for (auto col = 0; col < width; col++) {
    for (auto row = 0; row < height; row++) {
      m_cells[col][row].neighbourMinesCount = countNeighbourMines(col, row);
    }
  }
}

void Model::revealAllMines() {
  for (auto &col : m_cells) {
    for (auto &cell : col) {
      if (cell.type == Cell::Type::Mine) {
        cell.status = Cell::Status::Revealed;
      }
    }
  }
}

int Model::countNeighbourMines(int col, int row) {
  auto neighbourMinesCount{0};
  auto gridSize{sizeAsPair(m_size)};
  auto width{gridSize.first};
  auto height{gridSize.second};
  for (auto j = col - 1; j <= col + 1; j++) {
    for (auto z = row - 1; z <= row + 1; z++) {
      if ((j >= 0 && j < width) && (z >= 0 && z < height)) {
        if (m_cells[j][z].type == Cell::Type::Mine) {
          neighbourMinesCount++;
        }
      }
    }
  }
  return neighbourMinesCount;
}
