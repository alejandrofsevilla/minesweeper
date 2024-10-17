#include "Model.hpp"

#include <random>

#include "Cell.hpp"

namespace {
inline size_t generateRandomValue(size_t min, size_t max) {
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<size_t> distr{min, max};
  return distr(gen);
}

inline size_t minesCount(Model::Size size) {
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

inline std::pair<size_t, size_t> sizeAsPair(Model::Size size) {
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
    : m_status{Status::Ready}, m_size{Size::Size30x16}, m_cells{},
      m_startTime{} {}

Model::Size Model::size() const { return m_size; }

Model::Status Model::status() const { return m_status; }

size_t Model::width() const { return sizeAsPair(m_size).first; }

size_t Model::height() const { return sizeAsPair(m_size).second; }

size_t Model::markedMinesCount() const { return m_markedMinesCount; };

size_t Model::timeInSeconds() const { return m_timeInSeconds; }

const std::vector<std::vector<Cell>> &Model::cells() const { return m_cells; }

void Model::update() {
  switch (m_status) {
  case Status::Ready:
    if (m_cells.empty()) {
      generateCells();
      generateMines();
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

void Model::markMine(std::size_t col, std::size_t row) {
  m_cells[col][row].status = Cell::Status::Flagged;
}

void Model::markSuspect(std::size_t col, std::size_t row) {
  m_cells[col][row].status = Cell::Status::Suspected;
}

void Model::reveal(std::size_t col, std::size_t row) {
  auto &cell = m_cells[col][row];
  cell.status = Cell::Status::Revealed;
  if (cell.type == Cell::Type::Mine) {
    m_status = Status::Stopped;
    return;
  }
  auto neighbourMinesCount{cell.neighbourMinesCount};
  if (neighbourMinesCount == 0) {
    auto gridSize{sizeAsPair(m_size)};
    auto width{gridSize.first};
    auto height{gridSize.second};
    for (size_t i = col - 1; i <= col + 1; i++) {
      for (size_t j = row - 1; j <= row + 1; j++) {
        if ((i > 0 && i < width) && (j > 0 && j < height)) {
          reveal(i, j);
        }
      }
    }
  }
  m_status = Status::Started;
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
  generateCells();
  generateMines();
  m_status = Status::Ready;
}

void Model::updateTime() {
  m_timeInSeconds =
      static_cast<size_t>(std::chrono::duration_cast<std::chrono::seconds>(
                              std::chrono::system_clock::now() - m_startTime)
                              .count());
}

void Model::generateCells() {
  auto gridSize{sizeAsPair(m_size)};
  auto width{gridSize.first};
  auto height{gridSize.second};
  for (size_t col = 0; col < width; col++) {
    m_cells.push_back({});
    for (size_t row = 0; row < height; row++) {
      m_cells[col].push_back(
          {col, row, 0, Cell::Type::Empty, Cell::Status::Empty});
    }
  }
}

void Model::generateMines() {
  auto gridSize{sizeAsPair(m_size)};
  auto width{gridSize.first};
  auto height{gridSize.second};
  auto numberOfMines{minesCount(m_size)};
  for (size_t i = 0; i < numberOfMines; i++) {
    auto pos{generateRandomValue(0, width * height - 1)};
    auto col = pos % width;
    auto row = pos / width;
    m_cells[col][row].type = Cell::Type::Mine;
    for (size_t j = col - 1; j <= col + 1; j++) {
      for (size_t z = row - 1; z <= row + 1; z++) {
        if ((j > 0 && j < width) && (z > 0 && z < height)) {
          m_cells[j][z].neighbourMinesCount++;
        }
      }
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
