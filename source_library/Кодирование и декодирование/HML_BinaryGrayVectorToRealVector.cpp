void HML_BinaryGrayVectorToRealVector(int *x, int n, double *VHML_ResultVector, double *Left, double *Right, int *Lengthi, int VHML_N)
{
/*
Функция декодирует бинарную строку в действительный вектор, который и был закодирован
методом "Стандартный рефлексивный Грей-код" (без использования временного массива).
Входные параметры:
 a - бинарная строка представляющая собой Грей-код нескольких закодированных вещественных координат;
 n - длина бинарной строки;
 VHML_ResultVector - вещественный вектор, в который мы и записываем результат, размера n;
 Left - массив левых границ изменения каждой вещественной координаты (размер VHML_N);
 Right - массив правых границ изменения каждой вещественной координаты (размер VHML_N);
 Lengthi - массив значений, сколько на каждую координату отводится бит в бинарной строке (размер массива Lengthi VHML_N);
 VHML_N - длина вещественного вектора.
Возвращаемое значение:
 Отсутствует.
Примечание:
 К криптографии данная функция не имеет отношения.
*/
int len;//Сколько на текущую координату отводится бит в бинарной строке
int Begin=0;//Номер бита в бинарной строке, с которой начинается текущая закодированная вещественная координата
int *TempBinaryVector;
TempBinaryVector=new int[n];

//Вначале нужно перевести бинарную строку кода Грея в бинарную строку
for (int i=0;i<VHML_N;i++)
 {
 len=Lengthi[i];
 HML_GrayCodeToBinaryFromPart(x,TempBinaryVector,Begin,len);
 Begin+=len;
 }

//Переводим наш полученный бинарный код в вещественный вектор
HML_BinaryVectorToRealVector(TempBinaryVector,VHML_ResultVector,Left,Right,Lengthi,VHML_N);

delete [] TempBinaryVector;
}
//---------------------------------------------------------------------------
void HML_BinaryGrayVectorToRealVector(int *x, double *VHML_ResultVector,int *TempBinaryVector, double *Left, double *Right, int *Lengthi, int VHML_N)
{
/*
Функция декодирует бинарную строку в действительный вектор, который и был закодирован
методом "Стандартный рефлексивный Грей-код".
Входные параметры:
 a - бинарная строка представляющая собой Грей-код нескольких закодированных вещественных координат;
 VHML_ResultVector - вещественный вектор, в который мы и записываем результат;
 TempBinaryVector - указатель на временный массив  размера n;
 Left - массив левых границ изменения каждой вещественной координаты размера VHML_N;
 Right - массив правых границ изменения каждой вещественной координаты размера VHML_N;
 Lengthi - массив значений, сколько на каждую координату отводится бит в бинарной строке. Размер массива VHML_N;
 VHML_N - длина вещественного вектора.
Возвращаемое значение:
 Отсутствует.
Примечание:
 К криптографии данная функция не имеет отношения.
*/
int len;//Сколько на текущую координату отводится бит в бинарной строке
int Begin=0;//Номер бита в бинарной строке, с которой начинается текущая закодированная вещественная координата

//Вначале нужно перевести бинарную строку кода Грея в бинарную строку
for (int i=0;i<VHML_N;i++)
 {
 len=Lengthi[i];
 HML_GrayCodeToBinaryFromPart(x,TempBinaryVector,Begin,len);
 Begin+=len;
 }

//Переводим наш полученный бинарный код в вещественный вектор
HML_BinaryVectorToRealVector(TempBinaryVector,VHML_ResultVector,Left,Right,Lengthi,VHML_N);
}