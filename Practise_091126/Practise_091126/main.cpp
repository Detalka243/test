#include <utility>
#include <cstddef>
#include <limits>
#include <stdexcept>
class Ints {
public:
  Ints();
  Ints(size_t size, int value);
  Ints(const int *arr, size_t arr_size);
  Ints(Ints const& rhs);
  Ints(const size_t size);
  Ints(Ints && rhs) noexcept;

  Ints & operator=(const Ints & rhs);
  Ints & operator=(Ints && rhs) noexcept;
  Ints operator+(const Ints & rhs) const;

  ~Ints();

  size_t getSize() const noexcept;

  void set(size_t id, int i);
  int get(size_t id) const noexcept;

  int getSafe(size_t id) const;
  void setSafe(size_t id, int i);

  Ints append(int i) const;
  Ints append(const Ints &b) const;

private:
  size_t size;
  size_t capacity;
  int * data;
};

Ints::Ints():
  size(0),
  capacity(0),
  data(nullptr)
{}

Ints::Ints(const size_t size, const int value):
  size(size), capacity(size), data(new int[capacity])
{
  for (size_t i = 0; i < size; i++)
  {
    data[i] = value;
  }
}

Ints::Ints(const int * arr, const size_t arr_size):
  size(arr_size), capacity(size), data(new int[capacity])
{
  for (size_t i = 0; i < arr_size; i++)
  {
    data[i] = arr[i];
  }
}

Ints::Ints(const size_t size):
  size(size), capacity(size), data(new int[capacity])
{}

Ints::Ints(Ints const& rhs):
  size(rhs.size), capacity(rhs.capacity), data(new int [capacity])
{
  for (size_t i = 0; i < size; i++)
  {
    data[i] = rhs.data[i];
  }
}

Ints::Ints(Ints && rhs) noexcept:
  size(rhs.size),
  capacity(rhs.capacity),
  data(rhs.data)
{
  rhs.capacity = 0;
  rhs.size = 0;
  rhs.data = nullptr;
}

Ints & Ints::operator=(const Ints & rhs)
{
  if (this == &rhs)
  {
    return *this;
  }

  Ints temp(rhs);
  std::swap(size, temp.size);
  std::swap(data, temp.data);
  std::swap(capacity, temp.capacity);

  return *this;
}

Ints & Ints::operator=(Ints && rhs) noexcept
{
  if (this != &rhs)
  {
    delete [] data;

    capacity = rhs.capacity;
    size = rhs.size;
    data = rhs.data;

    rhs.data = nullptr;
    rhs.capacity = 0;
    rhs.size = 0;
  }

  return *this;
}

Ints Ints::operator+(const Ints & rhs) const
{
  if (getSize() != rhs.getSize())
  {
    throw std::invalid_argument("Arrays have diff size");
  }

  Ints result(getSize(), 0);
  for (size_t i = 0; i < getSize(); i++)
  {
    if (rhs.get(i) > std::numeric_limits< int >::max() - get(i) || rhs.get(i) < std::numeric_limits< int >::min() + get(i))
    {
      throw std::invalid_argument("Overflow");
    }
    result.set(i, rhs.get(i) + get(i));
  }

  return result;
}

Ints::~Ints()
{
  delete [] data;
}

void Ints::set(const size_t id, const int i)
{
  data[id] = i;
}

void Ints::setSafe(const size_t id, const int i)
{
  if (id >= size)
  {
    throw std::logic_error("Out of range");
  }

  set(id, i);
}

int Ints::get(const size_t id) const noexcept
{
  return data[id];
}

int Ints::getSafe(const size_t id) const
{
  if (id >= size)
  {
    throw std::logic_error("Out of range");
  }
  return get(id);
}

size_t Ints::getSize() const noexcept
{
  return size;
}

Ints Ints::append(const int i) const
{
  if (std::numeric_limits< size_t >::max() - 1 < size)
  {
    throw std::logic_error("Overflow");
  }

  Ints temp(size+1);
  size_t p = 0;

  while (p < size)
  {
    temp.data[p] = data[p];
    p++;
  }
  temp.data[p] = i;

  return temp;
}

Ints Ints::append(const Ints &b) const
{
  if (std::numeric_limits< size_t >::max() - size < b.size)
  {
    throw std::logic_error("Overflow");
  }

  Ints temp(size+b.size);
  size_t p = 0;

  while (p < size)
  {
    temp.data[p] = data[p];
    p++;
  }
  for (size_t i = 0; i < b.size; i++)
  {
    temp.data[p++] = b.data[i];
  }

  return temp;
}
  
class Mtx
{
public:
  Mtx();
  Mtx(size_t m, size_t n);
  Mtx(size_t m, size_t n, const Ints & data);
  ~Mtx() = default;

  Mtx sum(const Mtx & oth) const;
  Mtx mul(const Mtx & rhs) const;
  Ints getCol(size_t id) const;
  Ints getRow(size_t id) const;
  Mtx addRow(const int filler) const;
  Mtx transpose() const;
  int det() const;
private:
  size_t m;
  size_t n;
  Ints data;
};


Mtx::Mtx():
  m(0),
  n(0)
{}

Mtx::Mtx(size_t m, size_t n):
  m(m),
  n(n),
  data(n * m, 0)
{}
Mtx::Mtx(size_t m, size_t n, const Ints & data):
  m(m),
  n(n),
  data(data)
{}

Mtx Mtx::sum(const Mtx & oth) const
{
  Mtx res(m, n);
  for (size_t i = 0; i < data.getSize(); ++i)
  {
    if (oth.data.get(i) > std::numeric_limits< size_t >::max() - data.get(i))
    {
      throw std::logic_error("Owerflow");
    }

    res.data.set(i, oth.data.get(i) + data.get(i));
  }

  return res;
}

Ints Mtx::getCol(size_t id) const
{
  Ints res(m, 0);
  for (size_t i = 0; i < m; ++i)
  {
    res.append(i * n + id);
  }

  return res;
}

Ints Mtx::getRow(size_t id) const
{
  Ints res(n, 0);
  for (size_t i = 0; i < n; ++i)
  {
    res.append(id * m + i);
  }

  return res;
}

Mtx Mtx::mul(const Mtx & rhs) const
{
  Mtx res(m, rhs.n);
  for (size_t i = 0; i < m; i++)
  {
    for (size_t j = 0; j < rhs.n; j++)
    {
      Ints row = getRow(i);
      Ints col = rhs.getCol(j);

      int sm = 0;
      for (size_t k = 0; k < rhs.m; k++)
      {
        sm += row.get(k) * col.get(k);
      }

      res.data.set(i * rhs.n + j, sm);
    }
  }

  return res;
}
Mtx Mtx::addRow(const int filler) const
{
  Mtx res(m + 1, n);
  for (size_t i = 0; i < m * n; i++)
  {
    res.data.set(i, data.get(i));
  }
  for (size_t i = 0; i < n; i++)
  {
    res.data.set(n*m + i, filler);
  }
  return res;
}
Mtx Mtx::transpose() const
{
  Mtx res(n, m);

  for (size_t i = 0; i < m; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      res.data.set(j * m + i, data.get(i * n + j));
    }
  }
  return res;
}
int Mtx::det() const
{
  if (m != n) 
  {
    throw std::logic_error("Can't estimate determinant");
  }
  if (m == 1)
  {
    return data.get(0);
  }
  if (m == 2)
  {
    return data.get(0) * data.get(3) - data.get(1) * data.get(2);
  }

  int res = 0;

  for (size_t i = 0; i < n; i++) 
  {
    Mtx minor(m - 1, n - 1);

    size_t p = 0;

    for (size_t row = 1; row < m; row++)
    {
      for (size_t col = 0; col < n; col++)
      {
        if (col == i)
        {
          continue;
        }

        minor.data.set(p++, data.get(row * n + col));
      }
    }
    int sign = 0;
    if (i % 2 == 0)
    { 
      sign = 1;
    }
    else
    {
      sign = -1;
    }

    res += sign * data.get(i) * minor.det();
  }

  return res;
}
struct p_t
{
  int x;
  int y;
};
class Points
{
  public:
    Points():
      data_(Ints())
    {}

    Points(const Ints &data):
      data_(data)
    {}

    ~Points() = default;

    void set(const size_t id, const p_t p) noexcept
    {
      data_.set(id * 2, p.x);
      data_.set(id * 2 + 1, p.y);
    }

    int get(const size_t id) const noexcept
    {
      return data_.get(id);
    }

    Points append(p_t p) const
    {
      size_t const size = data_.getSize();

      if (size > std::numeric_limits<size_t>::max() - 1)
      {
        throw std::logic_error("Overflow");
      }

      Ints tmp(size + 2);

      for (size_t i = 0; i < size; i++)
      {
        tmp.set(i, data_.get(i));
      }

      tmp.set(size, p.x);
      tmp.set(size + 1, p.y);

      return tmp;
    }
  private:
    Ints data_;
};
class Plot
{
public:
  void set(int x, int y);
  int calc(int x) const;
  size_t solve(Ints & buf, int y) const;
  Points append(p_t p) const;
private:
  Ints data_;
};
size_t Plot::solve(Ints & buf, const int y) const
{
  size_t cnt = 0;
  for (size_t i = 1; i < data_.getSize(); i+=2)
  {
    if (y == data_.get(i))
    {
      buf.append(data_.get(i - 1));
      ++cnt;
    }
  }

  return cnt;
}
void Plot::set(const int x, const int y)
{
  for (size_t i = 0; i < data_.getSize(); i+=2)
  {
    if (x == data_.get(i))
    {
      data_.setSafe(i + 1, y);
      break;
    }
  }
  data_ = data_.append(x);
  data_ = data_.append(y);
}
int Plot::calc(const int x) const
{
  for (size_t i = 0; i < data_.getSize(); i+=2)
  {
    if (data_.get(i) == x)
    {
      return data_.get(i + 1);
    }
  }
  throw std::logic_error("Not found");
}
Points Plot::append(const p_t p) const
{
  size_t size = data_.getSize();
  Ints tmp(size + 2);
  for (size_t i = 0; i < size; ++i)
  {
    tmp.set(i, data_.get(i));
  }
  tmp.set(size, p.x);
  tmp.set(size + 1, p.y);
  
  return tmp;
}
int main()
{
  Ints a(10, 67);
  Ints b(10, 69);
  Ints c = a + b + a + b + a;
}
