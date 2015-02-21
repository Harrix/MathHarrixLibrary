template <class T> void HML_RandomIntMatrix(T **VHML_ResultMatrix, T n, T m, int VHML_N, int VHML_M)
{
/*
Функция заполняет матрицу случайными целыми числами из определенного интервала [n;m).
Входные параметры:
 VHML_ResultMatrix - указатель на матрицу;
 n - левая граница интервала;
 m - правая граница интервала;
 VHML_N - размер массива (число строк);
 VHML_M - размер массива (число столбцов).
Возвращаемое значение:
 Отсутствует.
*/
for (int i=0;i<VHML_N;i++)
 for (int j=0;j<VHML_M;j++)
  VHML_ResultMatrix[i][j]=HML_RandomUniformInt(n,m);
}