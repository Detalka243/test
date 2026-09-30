#include <iostream>
void rmMtx(int ** mtx, const size_t m);
void printMtx();
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

  int **mtx = nullptr;

  for (size_t i = 0; i < m * n; ++i)
  {
    std::cin >> mtx[i % m][i / m];
  }
  

}
