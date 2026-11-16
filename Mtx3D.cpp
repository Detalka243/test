#include <algorithm>
#include <stdexcept>
#include <limits>
#include <cstddef>

class Mtx3d
{
  public:
    Mtx3d();
    Mtx3d(const Mtx3d & rhs);
    Mtx3d(Mtx3d && rhs) noexcept;
    Mtx3d(size_t n, size_t m, size_t k, int fill = 0);
    Mtx3d & operator=(const Mtx3d & rhs);
    Mtx3d & operator=(Mtx3d && rhs) noexcept;
    int** operator[](size_t i);
    const int* const* operator[](size_t i) const;
    Mtx3d operator+(const Mtx3d & rhs) const;
    ~Mtx3d();
    size_t getN() const;
    size_t getM() const;
    size_t getK() const;

    Mtx3d eraseX(size_t l, size_t r = 0);
    Mtx3d eraseY(size_t l, size_t r = 0);
    Mtx3d eraseZ(size_t l, size_t r = 0);
    Mtx3d erase(size_t r, size_t l = 0);

    Mtx3d insertX(const Mtx3d & Mtx, size_t id);
    Mtx3d insertY(const Mtx3d & Mtx, size_t id);
    Mtx3d insertZ(const Mtx3d & Mtx, size_t id);
    Mtx3d insert(const Mtx3d & Mtx, size_t id);

    int conv(int (*func)(int a, int b));
  private:
    size_t n;
    size_t m;
    size_t k;
    int *** mtx;
    void clearMtx() noexcept;
};
size_t Mtx3d::getN() const
{
  return n;
}
size_t Mtx3d::getM() const
{
  return m;
}
size_t Mtx3d::getK() const
{
  return k;
}
void Mtx3d::clearMtx() noexcept
{
  if (mtx == nullptr)
  {
    return;
  }
  for (size_t i = 0; i < n; i++)
  {
    if (mtx[i] == nullptr)
    {
      continue;
    }
    for (size_t j = 0; j < m; j++)
    {
      delete[] mtx[i][j];
    }
    delete[] mtx[i];
  }
  delete[] mtx;

  mtx = nullptr;
}
Mtx3d::Mtx3d():
  n(1),
  m(1),
  k(1),
  mtx(nullptr)
{
  try
  {
    mtx = new int** [n]{};
    mtx[0] = new int* [m]{};
    mtx[0][0] = new int [k]{};
    mtx[0][0][0] = 0;
  } catch (...) {
    clearMtx();
    throw;
  }
}
Mtx3d::Mtx3d(const Mtx3d & rhs):
  n(rhs.n),
  m(rhs.m),
  k(rhs.k),
  mtx(nullptr)
{
  try
  {
    mtx = new int ** [n]{};
    for (size_t i = 0; i < n; ++i)
    {
      mtx[i] = new int * [m]{};
      for (size_t j = 0; j < m; ++j)
      {
        mtx[i][j] = new int [k]{};
        for (size_t p = 0; p < k; ++p)
        {
          mtx[i][j][p] = rhs[i][j][p];
        }
      }
    }
  } catch (...) {
    clearMtx();
    throw;
  }
}
Mtx3d::Mtx3d(size_t n, size_t m, size_t k, int fill):
  n(n),
  m(m),
  k(k),
  mtx(nullptr)
{
  try
  {
    mtx = new int ** [n]{};
    for (size_t i = 0; i < n; ++i)
    {
      mtx[i] = new int * [m]{};
      for (size_t j = 0; j < m; ++j)
      {
        mtx[i][j] = new int[k]{};
        for (size_t p = 0; p < k; ++p)
        {
          mtx[i][j][p] = fill;
        }
      }
    }
  } catch (...) {
    clearMtx();
    throw;
  }
}
Mtx3d::Mtx3d(Mtx3d && rhs) noexcept:
  n(rhs.n),
  m(rhs.m),
  k(rhs.k),
  mtx(rhs.mtx)
{
  rhs.n = 0;
  rhs.m = 0;
  rhs.k = 0;
  rhs.mtx = nullptr;
}
Mtx3d & Mtx3d::operator=(const Mtx3d & rhs)
{
  Mtx3d temp(rhs);
  std::swap(n, temp.n);
  std::swap(m, temp.m);
  std::swap(k, temp.k);
  std::swap(mtx, temp.mtx);
  return *this;
}
Mtx3d & Mtx3d::operator=(Mtx3d && rhs) noexcept
{
  if (this != &rhs)
  {
    std::swap(n, rhs.n);
    std::swap(m, rhs.m);
    std::swap(k, rhs.k);
    std::swap(mtx, rhs.mtx);
  }

  return *this;
}
Mtx3d Mtx3d::operator+(const Mtx3d & rhs) const
{
  if (n != rhs.n || m != rhs.m || k != rhs.k)
  {
    throw std::invalid_argument("Mtxs have different sizes");
  }
  Mtx3d res(*this);
  int first = 0;
  int second = 0;
  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      for (size_t l = 0; l < k; l++)
      {
        first = res[i][j][l];
        second = rhs[i][j][l];
        if ((second > 0 && first > std::numeric_limits<int>::max() - second) ||
          (second < 0 && first < std::numeric_limits<int>::min() - second))
        {
          throw std::logic_error("Overflow");
        }
        res[i][j][l] = first + second;
      }
    }
  }
  return res;
}
int** Mtx3d::operator[](size_t i)
{
  return mtx[i];
}

const int* const* Mtx3d::operator[](size_t i) const
{
  return mtx[i];
}
Mtx3d Mtx3d::eraseX(size_t l, size_t r)
{
  if (r == 0)
  {
    r = l + 1;
  }
  if (l >= r || r > m)
  {
    throw std::logic_error("");
  }
  if (m - r > std::numeric_limits<size_t>::max() - l)
  {
    throw std::logic_error("");
  }
  Mtx3d res(n, m - r + l, k);

  for (size_t i = 0; i < n; i++)
  {
    size_t index = 0;
    for (size_t j = 0; j < m; j++)
    {
      if (j < r && j >= l)
      {
        continue;
      }
      for(size_t p = 0; p < k; p++)
      {
        res[i][index][p] = mtx[i][j][p];
      }
      index++;
    }
  }
  return res;
}
Mtx3d Mtx3d::eraseY(size_t l, size_t r)
{
  if (r == 0)
  {
    r = l + 1;
  }
  if (l >= r || r > n)
  {
    throw std::logic_error("");
  }
  if (n - r > std::numeric_limits<size_t>::max() - l)
  {
    throw std::logic_error("");
  }
  Mtx3d res(n - r + l, m, k);
  size_t index = 0;
  for (size_t i = 0; i < n; i++)
  {
    if (i < r && i >= l)
    {
      continue;
    }
    for (size_t j = 0; j < m; j++)
    {
      for(size_t p = 0; p < k; p++)
      {
        res[index][j][p] = mtx[i][j][p];
      }
    }
    index++;
  }
  return res;
}
Mtx3d Mtx3d::eraseZ(size_t l, size_t r)
{
  if (r == 0)
  {
    r = l + 1;
  }
  if (l >= r || r > k)
  {
    throw std::logic_error("");
  }
  if (k - r > std::numeric_limits<size_t>::max() - l)
  {
    throw std::logic_error("");
  }
  Mtx3d res(n, m, k - r + l);

  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      size_t index = 0;

      for(size_t p = 0; p < k; p++)
      {
        if (p >= l && p < r)
        {
          continue;
        }
        res[i][j][index] = mtx[i][j][p];
        index++;
      }
    }
  }
  return res;
}
Mtx3d Mtx3d::erase(size_t r, size_t l)
{
  if (r > std::numeric_limits<size_t>::max() - l ||
      r + l > n || r + l > m || r + l > k)
  {
    throw std::logic_error("");
  }
  Mtx3d res = this->eraseX(m-r, m).eraseY(n-r, n).eraseZ(k-r, k);
  if (l)
  {
    res = res.eraseX(0, l).eraseY(0, l).eraseZ(0, l);
  }
  return res;
}
Mtx3d Mtx3d::insertX(const Mtx3d & Mtx, size_t id)
{
  if (id > m || Mtx.getN() != n || Mtx.getK() != k ||
      Mtx.getM() > std::numeric_limits<size_t>::max() - m) {
    throw std::logic_error("");
  }
  Mtx3d res(n, Mtx.getM() + m, k);
  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < id; j++)
    {
      for (size_t l = 0; l < k; l++)
      {
        res[i][j][l] = mtx[i][j][l];
      }
    }
  }
  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < Mtx.getM(); j++)
    {
      for (size_t l = 0; l < k; l++)
      {
        res[i][j + id][l] = Mtx[i][j][l];
      }
    }
  }
  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = id; j < m; j++)
    {
      for (size_t l = 0; l < k; l++)
      {
        res[i][j + Mtx.getM()][l] = mtx[i][j][l];
      }
    }
  }
  return res;
}
Mtx3d Mtx3d::insertY(const Mtx3d & Mtx, size_t id)
{
  if (id > n || Mtx.getM() != m || Mtx.getK() != k ||
      Mtx.getN() > std::numeric_limits<size_t>::max() - n) {
    throw std::logic_error("");
  }
  Mtx3d res(n + Mtx.getN(), m, k);
  for (size_t i = 0; i < id; i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      for (size_t l = 0; l < k; l++)
      {
        res[i][j][l] = mtx[i][j][l];
      }
    }
  }
  for (size_t i = 0; i < Mtx.getN(); i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      for (size_t l = 0; l < k; l++)
      {
        res[i + id][j][l] = Mtx[i][j][l];
      }
    }
  }
  for (size_t i = id; i < n; i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      for (size_t l = 0; l < k; l++)
      {
        res[i + Mtx.getN()][j][l] = mtx[i][j][l];
      }
    }
  }
  return res;
}

Mtx3d Mtx3d::insertZ(const Mtx3d & Mtx, size_t id)
{
  if (id > k || Mtx.getM() != m || Mtx.getN() != n ||
      Mtx.getK() > std::numeric_limits<size_t>::max() - k)
  {
    throw std::logic_error("");
  }
  Mtx3d res(n, m, k + Mtx.getK());
  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      for (size_t l = 0; l < id; l++)
      {
        res[i][j][l] = mtx[i][j][l];
      }
    }
  }
  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      for (size_t l = 0; l < Mtx.getK(); l++)
      {
        res[i][j][l + id] = Mtx[i][j][l];
      }
    }
  }
  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      for (size_t l = id; l < k; l++)
      {
        res[i][j][l + Mtx.getK()] = mtx[i][j][l];
      }
    }
  }
  return res;
}


Mtx3d Mtx3d::insert(const Mtx3d & Mtx, size_t id)
{
  if (Mtx.getN() == n && Mtx.getK() == k)
  {
    return this->insertX(Mtx, id);
  }
  else if (Mtx.getM() == m && Mtx.getK() == k)
  {
    return this->insertY(Mtx, id);
  }
  else if (Mtx.getN() == n && Mtx.getM() == m)
  {
    return this->insertZ(Mtx, id);
  }
  throw std::logic_error("");
}

int Mtx3d::conv(int (*func)(int a, int b))
{
  if (func == nullptr)
  {
    throw std::invalid_argument("Null func");
  }
  if (n == 0 || m == 0 || k == 0)
  {
    throw std::logic_error("Mtx is empty");
  }
  int temp = mtx[0][0][0];
  for (size_t l = 1; l < k; l++)
  {
    temp = func(temp, mtx[0][0][l]);
  }
  for (size_t j = 1; j < m; j++)
  {
    for (size_t l = 0; l < k; l++)
    {
      temp = func(temp, mtx[0][j][l]);
    }
  }
  for (size_t i = 1; i < n; i++)
  {
    for (size_t j = 0; j < m; j++)
    {
      for (size_t l = 0; l < k; l++)
      {
        temp = func(temp, mtx[i][j][l]);
      }
    }
  }
  return temp;
}
Mtx3d::~Mtx3d()
{
  clearMtx();
}
int main()
{

}
