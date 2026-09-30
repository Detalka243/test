#include <iostream>
int ** convert(const int * t, size_t n, const size_t * lns, size_t rows)
{
  int ** result = new int * [rows];
  size_t index = 0;
  for (size_t i = 0; i < rows; ++i)
  {
    result[i] = new int[lns[i]];
    for (size_t j = 0; j < lns[i]; ++j)
    {
      if (index < n) 
      {
        result[i][j] = t[index];
        index++;
      }
      else
      {
        result[i][j] = 0;
      }
    }
  }
  return result;
}
int main()
{

}
