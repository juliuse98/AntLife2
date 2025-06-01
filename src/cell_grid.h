#include <cmath>
#include <cstddef>
#include <vector>

template <typename T> class CellGrid {
public:
  CellGrid(float size, unsigned int divLevel)
      : m_cells(pow4(divLevel)), m_size(size), m_rows(pow2(divLevel)),
        m_cellSize(size / m_rows) {}

  T getC(int x, int y) const { return m_cells.at(coordToIndex(x, y)); }
  T get(float x, float y) const { return m_cells.at(pointToIndex(x, y)); }

  void setC(int x, int y, T value) { m_cells[coordToIndex(x, y)] = value; };
  void set(float x, float y, T value) { m_cells[pointToIndex(x, y)] = value; };
  static size_t pow2(int x) { return static_cast<size_t>(std::pow(2.0, x)); }
  static size_t pow4(int x) { return static_cast<size_t>(std::pow(4.0, x)); }
  int coordToIndex(int x, int y) const { return y * m_rows + x; };
  int pointToIndex(float x, float y) const {
    int col = static_cast<int>(x / m_cellSize);
    int row = static_cast<int>(y / m_cellSize);
    return coordToIndex(col, row);
  };

private:
  float m_size;
  unsigned int m_rows;
  float m_cellSize;
  std::vector<T> m_cells;
};
