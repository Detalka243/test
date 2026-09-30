#include <iostream>

void rmMtx(int ** mtx, const size_t m)
{
  for (size_t i = 0; i < m; ++i) 
  {
    delete [] mtx[i];
  }
  delete [] mtx;
} 
void printMtx(const size_t m, const size_t n)
{
  std::cout << mtx[0][0];
  for (size_t i = 1; i < n; ++i)
  {
    std::cout << "_" << mtx[0][i];
  }

  for (size_t i = 1; i < m; ++i)
  {
    std::cout << "\n" << mtx[i][0];
    for (size_t j = 1; j < n; ++j)
    {
      std::cout << "_" << mtx[i][j];
    }
  }
  std::cout << "\n";
}
int ** makeMtx(const size_t m, const size_t n)
{
  int **mtxR = new int *[m];
  try
  {
    for (size_t i = 0; i < m; ++i)
    {
      mtxR[i] = new int [n];
    }
  }
  catch (const std::bad_alloc &e) 
  {
    rmMtx(mtxR, m);
  }
  return mtxR;
}
int ** transpose();

int main()
{
  size_t m = 0;
  size_t n = 0;
  std::cin >> m >> n;
  
  if ((!std::cin) || m == 0 || n == 0)
  {
    return 1;
  }

  int **mtx = nullptr;
  
  try
  {
    mtx = makeMtx(m, n);
  }
  catch (...)
  {
    return 2;
  }


  for (size_t i = 0; i < m * n; ++i)
  {
    std::cin >> mtx[i % m][i / m];
  }
  if (std::cin.fail()) 
  {
    rmMtx(mtx, m);
    return 1;
  }
  std::cout << "\n";
  printMtx(m, n);
  
  rmMtx(mtx, m);
}
