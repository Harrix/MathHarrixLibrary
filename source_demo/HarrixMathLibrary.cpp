//Библиотека HarrixMathLibrary
//Версия 3.16

//Сборник различных математических функций с открытым кодом на языке C++
//Страница проекта: https://github.com/Harrix/HarrixMathLibrary
//Распространяется по лицензии Apache License, Version 2.0

//   Copyright 2007-2013 Harrix
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "HarrixMathLibrary.h"

//ДЛЯ ГЕНЕРАТОРА СЛУЧАЙНЫХ ЧИСЕЛ
unsigned int MHL_Dummy;//Результат инициализации генератора случайных чисел

//СЛУЖЕБНЫЕ ДОПОЛНИТЕЛЬНЫЕ ПЕРМЕННЫЕ
double (*VMHL_TempFunction)(double*,int);
int *VMHL_TempInt1;
int *VMHL_TempInt2;
int *VMHL_TempInt3;
int *VMHL_TempInt4;
double *VMHL_TempDouble1;
double *VMHL_TempDouble2;
double *VMHL_TempDouble3;

TypeOfTestFunction VMHL_TypeOfTestFunction;//для функций по работе с тестовыми функциями для анализа эффективности алгоритмов
int CountOfFitness;//количество вызовов тестовой функции при запуске алгоритма оптимизации

void MHL_SeedRandom(void)
{
/*
Инициализатор генератора случайных чисел.
В данном случае используется самый простой его вариант со всеми его недостатками.
Входные параметры:
 Отсутствуют.
Возвращаемое значение:
 Отсутствуют.
*/
//В качестве начального значения для ГСЧ используем текущее время
MHL_Dummy=(unsigned)time(NULL);
srand(MHL_Dummy);//Стандартная инициализация
rand();//первый вызов для контроля
}
//---------------------------------------------------------------------------
double MHL_RandomNumber(void)
{
/*
Генератор случайных чисел (ГСЧ).
В данном случае используется самый простой его вариант со всеми его недостатками.
Использовать в функциях по криптографии не стоит.
Входные параметры:
 Отсутствуют.
Возвращаемое значение:
 Случайное вещественное число из интервала (0;1) по равномерному закону распределения.
*/
return (double)rand()/(RAND_MAX+1);
}
//---------------------------------------------------------------------------

//*****************************************************************
//Вектора (Одномерные массивы)
//*****************************************************************
void MHL_DependentNoiseInVector(double *VMHL_ResultVector, double percent, int VMHL_N)
{
/*
Функция добавляет к элементам выборки помеху, зависящую от значения элемента выборки
(плюс-минус сколько-то процентов модуля разности минимального и максимального элемента выборки,
умноженного на значение элемента).
Входные параметры:
 VMHL_ResultVector - указатель на массив;
 percent - процент шума;
 VMHL_N - количество элементов в массивах.
Возвращаемое значение:
 Отсутствует.
*/
if (percent<0) percent=0;
int i;
double max=TMHL_MaximumOfVector(VMHL_ResultVector,VMHL_N);//Максимальное значение
double min=TMHL_MinimumOfVector(VMHL_ResultVector,VMHL_N);//Минимальное значение
double b=percent*(max-min)/100.;//Амплитуда шума
for (i=0;i<VMHL_N;i++)
 VMHL_ResultVector[i]+=VMHL_ResultVector[i]*MHL_RandomUniform(-b/2.,b/2.);
}
//---------------------------------------------------------------------------
double MHL_EuclidNorma(double *a,int VMHL_N)
{
/*
Функция вычисляет евклидовую норму вектора.
Входные параметры:
 a - указатель на вектор;
 VMHL_N -  размер массива.
Возвращаемое значение:
 Значение евклидовой нормы вектора.
*/
int i;
double VMHL_Result=0;

for (i=0;i<VMHL_N;i++)
 VMHL_Result+=a[i]*a[i];

VMHL_Result=sqrt(double(VMHL_Result));
return VMHL_Result;
}
//---------------------------------------------------------------------------
void MHL_NoiseInVector(double *VMHL_ResultVector, double percent, int VMHL_N)
{
/*
Функция добавляет к элементам выборки аддитивную помеху (плюс-минус сколько-то процентов
модуля разности минимального и максимального элемента выборки).
Входные параметры:
 VMHL_ResultVector - указатель на массив;
 percent - процент шума;
 VMHL_N - количество элементов в массивах.
Возвращаемое значение:
 Отсутствует.
*/
if (percent<0) percent=0;
int i;
double max=TMHL_MaximumOfVector(VMHL_ResultVector,VMHL_N);//Максимальное значение
double min=TMHL_MinimumOfVector(VMHL_ResultVector,VMHL_N);//Минимальное значение
double b=percent*(max-min)/100.;//Амплитуда шума
for (i=0;i<VMHL_N;i++)
 VMHL_ResultVector[i]+=MHL_RandomUniform(-b/2.,b/2.);
}
//---------------------------------------------------------------------------

//*****************************************************************
//Генетические алгоритмы
//*****************************************************************
double MHL_BinaryFitnessFunction(int*x, int VMHL_N)
{
/*
Служебная функция. Функция вычисляет целевую функцию бинарного вектора, в котором
закодирован вещественный вектор. Использует внутренние служебные переменные.
Функция для MHL_StandartRealGeneticAlgorithm. Использовать для своих целей не рекомендуется.
Входные параметры:
 x - бинарный вектор;
 VMHL_N - количество элементов в векторе.
Возвращаемое значение:
 Значение целевой функции бинарного вектора.
Примечание. Используемые переменные, переодеваемые из MHL_StandartRealGeneticAlgorithm:
 VMHL_TempFunction - указатель на целевая функция для вещественного решения;
 VMHL_TempInt1 - указатель на массив, сколько бит приходится в бинарной хромосоме на кодирование ;
 VMHL_TempDouble1 - указатель на массив левых границ изменения вещественной переменной;
 VMHL_TempDouble2 - указатель на массив правых границ изменения вещественной переменной;
 VMHL_TempDouble3 - указатель на массив, в котором можно сохранить вещественный индивид при его раскодировании из бинарной строки;
 VMHL_TempInt2 - указатель на размерность вещественного вектора;
 VMHL_TempInt3 - указатель на тип преобразования вещественной задачи оптимизации в бинарное.
*/
double VMHL_Result;
int RealLength=*VMHL_TempInt2;//Размерность вещественного вектора
int TypOfConverting=*VMHL_TempInt3;//Тип преобразования

if (VMHL_N>0) VMHL_Result=0;//строчка только для того, чтобы компилятор не говорил, что VMHL_N не используется

//Переведем вектор из бинарного в вещественный
if (TypOfConverting==0)//IntConverting (Стандартное представление целого числа – номер узла в сетке дискретизации)
 MHL_BinaryVectorToRealVector(x,VMHL_TempDouble3,VMHL_TempDouble1,VMHL_TempDouble2,VMHL_TempInt1,RealLength);

if (TypOfConverting==1)//GrayСodeConverting (Стандартный рефлексивный Грей-код)
 MHL_BinaryGrayVectorToRealVector(x,VMHL_TempDouble3,VMHL_TempInt4,VMHL_TempDouble1,VMHL_TempDouble2,VMHL_TempInt1,RealLength);

//Посчитаем значение целевой функции вещественного вектора
VMHL_Result=VMHL_TempFunction(VMHL_TempDouble3,RealLength);

return VMHL_Result;
}
//---------------------------------------------------------------------------
void MHL_MakeVectorOfProbabilityForProportionalSelectionV2(double *Fitness, double *VMHL_ResultVector, int VMHL_N)
{
/*
Функция формирует вектор вероятностей выбора индивидов из вектора значений функции пригодности.
Формирование вектора происходит согласно правилам пропорционально селекции из ГА.
Это служебная функция для использования функции пропорциональной селекции MHL_ProportionalSelectionV2.
Входные параметры:
 Fitness - массив пригодностей (можно подавать не массив пригодностей, а массив значений целевой функции, но только для задач безусловной оптимизации);
 VMHL_ResultVector - вектор вероятностей выбора индивидов из популяции, который мы и формируем;
 VMHL_N - размер массива пригодностей.
Возвращаемое значение:
 Отстутствет.
*/
//Вектор Fitness мы не меняем. Поэтому проводим копирование.
TMHL_VectorToVector(Fitness,VMHL_ResultVector,VMHL_N);

//Проводим нормировку вектора, с целью получения вектора вероятностей.
//Нормировка вектора чисел такая, чтобы максимальный элемент имел значение 1, а минимальный 0.
//После данный нормализованный вектор сжимается так чтобы сумма всех элементов стала равна 1.
MHL_NormalizationVectorOne (VMHL_ResultVector,VMHL_N);
}
//---------------------------------------------------------------------------
void MHL_MakeVectorOfProbabilityForRanklSelection(double *Rank, double *VMHL_ResultVector, int VMHL_N)
{
/*
Функция формирует вектор вероятностей выбора индивидов из вектора рангов для ранговой селекции.
Это служебная функция для использования функции ранговой селекции MHL_RankSelection.
Входные параметры:
 Rank - массив рангов, которые были посчитаны функцией MHL_MakeVectorOfRankForRankSelection;
 VMHL_ResultVector - вектор вероятностей выбора индивидов из популяции, который мы и формируем;
 VMHL_N - размер массива пригодностей.
Возвращаемое значение:
 Отсутствует.
*/
//Вектор Fitness мы не меняем. Поэтому проводим копирование.
TMHL_VectorToVector(Rank,VMHL_ResultVector,VMHL_N);

//Проводим нормировку вектора, с целью получения вектора вероятностей.
double sum=TMHL_SumVector(VMHL_ResultVector,VMHL_N);
if (sum==0)
 {
 //Если сумма равна нулю
 for (int i=0;i<VMHL_N;i++) VMHL_ResultVector[i]=1./double(VMHL_N);
 }
else
 {
 for (int i=0;i<VMHL_N;i++) VMHL_ResultVector[i]/=sum;
 }
}
//---------------------------------------------------------------------------
void MHL_MakeVectorOfRankForRankSelection(double *Fitness, double *VMHL_ResultVector, int VMHL_N)
{
/*
Проставляет ранги для элементов не сортированного массива, то есть номера,
начиная с 1, в отсортированном массиве.  Если в массиве есть несколько одинаковых
элементов, то ранги им присуждаются как среднеарифметические.
Это служебная функция для функции MHL_RankSelection.
Входные параметры:
 Fitness - массив пригодностей (можно подавать не массив пригодностей, а массив значений целевой функции, но только для задач безусловной оптимизации);
 VMHL_ResultVector - массив рангов, который мы и формируем;
 VMHL_N - размер массива пригодностей.
Возвращаемое значение:
 Отсутствует.
*/
int j,i,k;
double Sn;
double *F;
int *N;
F=new double[VMHL_N];
N=new int[VMHL_N];

TMHL_VectorToVector(Fitness,F,VMHL_N);

//Заполним номерами
TMHL_OrdinalVectorZero(N,VMHL_N);

//сортируем массив пригодностей со сопряженным массивом номеров индивидов
TMHL_BubbleSortWithConjugateVector (F,N,VMHL_N);

//расставляем ранги
for (i=0;i<VMHL_N;i++)
 VMHL_ResultVector[N[i]]=i+1;

//для одинаковых элементов ранги делаем одинаковыми как среднее арифметическое
for (i=0;i<VMHL_N-1;i++)
 {
 if (F[i]==F[i+1])
  {
  j=i+1;
  while ((F[i]==F[j])&&(j<VMHL_N)) j++;
  Sn=MHL_SumOfArithmeticalProgression(i+1,1,j-i);
  Sn/=double(j-i);
  for (k=0;k<VMHL_N;k++)
   if (Fitness[k]==F[i]) VMHL_ResultVector[k]=Sn;
  i=j-1;
  }
 }
delete[] N;
delete[] F;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void MHL_MakeVectorOfRankZeroForRankSelection(double *Fitness, double *VMHL_ResultVector, int VMHL_N)
{
/*
Проставляет ранги для элементов не сортированного массива, то есть номера,
начиная с 0 (а не 1), в отсортированном массиве.  Если в массиве есть несколько одинаковых
элементов, то ранги им присуждаются как среднеарифметические.
Это модифицированная функция. Оригинальная функция MHL_MakeVectorOfRankForRankSelectionпроставляет ранги с 1.
Входные параметры:
 Fitness - массив пригодностей (можно подавать не массив пригодностей, а массив значений целевой функции, но только для задач безусловной оптимизации);
 VMHL_ResultVector - массив рангов, который мы и формируем;
 VMHL_N - размер массива пригодностей.
Возвращаемое значение:
 Отсутствует.
*/
int j,i,k;
double Sn;
double *F;
int *N;
F=new double[VMHL_N];
N=new int[VMHL_N];

TMHL_VectorToVector(Fitness,F,VMHL_N);

//Заполним номерами
TMHL_OrdinalVectorZero(N,VMHL_N);

//сортируем массив пригодностей со сопряженным массивом номеров индивидов
TMHL_BubbleSortWithConjugateVector (F,N,VMHL_N);

//расставляем ранги
for (i=0;i<VMHL_N;i++)
 VMHL_ResultVector[N[i]]=i;//VMHL_ResultVector[N[i]]=i+1;

//для одинаковых элементов ранги делаем одинаковыми как среднее арифметическое
for (i=0;i<VMHL_N-1;i++)
 {
 if (F[i]==F[i+1])
  {
  j=i+1;
  while ((F[i]==F[j])&&(j<VMHL_N)) j++;
  Sn=MHL_SumOfArithmeticalProgression(i,1,j-i);
  Sn/=double(j-i);
  for (k=0;k<VMHL_N;k++)
   if (Fitness[k]==F[i]) VMHL_ResultVector[k]=Sn;
  i=j-1;
  }
 }
delete[] N;
delete[] F;
}
//---------------------------------------------------------------------------
void MHL_NormalizationVectorAll(double *x,int VMHL_N)
{
/*
Нормировка вектора чисел в отрезок [0;1] посредством функции MHL_NormalizationNumberAll.
Входные параметры:
 VMHL_ResultVector - указатель на вектор (одномерный массив);
 VMHL_N - размер массива.
Возвращаемое значение:
 Отсутствует.
*/
for (int i=0;i<VMHL_N;i++)
 x[i]=MHL_NormalizationNumberAll(x[i]);
}
//---------------------------------------------------------------------------
void MHL_NormalizationVectorMaxMin(double *VMHL_ResultVector,int VMHL_N)
{
/*
Нормировка вектора чисел так, чтобы максимальный элемент имел значение 1, а минимальный 0.
Входные параметры:
 VMHL_ResultVector - указатель на вектор (одномерный массив);
 VMHL_N - размер массива.
Возвращаемое значение:
 Отсутствует.
*/
double max=TMHL_MaximumOfVector(VMHL_ResultVector,VMHL_N);//максимальное значение
double min=TMHL_MinimumOfVector(VMHL_ResultVector,VMHL_N);//минимальное значение
int vbool=0;
if (max<MHL_MINFINITY)
 {
 //Если все числа очень маленькие
 for (int i=0;i<VMHL_N;i++) VMHL_ResultVector[i]=1.0;
 vbool=1;
 }
if ((min>MHL_INFINITY)&&(vbool==0))
 {
 //Если все числа очень большие
 for (int i=0;i<VMHL_N;i++) VMHL_ResultVector[i]=1.0;
 vbool=1;
 }
if ((min==max)&&(vbool==0))
 {
 //Если все числа равны
 for (int i=0;i<VMHL_N;i++) VMHL_ResultVector[i]=1.0;
 vbool=1;
 }
if (vbool==0)
 {
 double d=max-min;
 for (int i=0;i<VMHL_N;i++)
  VMHL_ResultVector[i]=(VMHL_ResultVector[i]-min)/d;
 }
}
//---------------------------------------------------------------------------
void MHL_NormalizationVectorOne(double *VMHL_ResultVector,int VMHL_N)
{
/*
Нормировка вектора чисел в отрезок [0,1] так, чтобы сумма всех элементов была равна 1.
Входные параметры:
 VMHL_ResultVector - указатель на вектор (одномерный массив), который и будет преобразовываться;
 VMHL_N - размер массива.
Возвращаемое значение:
 Отсутствует.
*/
//вначале отнормируем в интервал
MHL_NormalizationVectorMaxMin (VMHL_ResultVector,VMHL_N);
//Вычислим сумму вектора
double sum=TMHL_SumVector(VMHL_ResultVector,VMHL_N);
if (sum==0)
 {
 //Если сумма равна нулю
 for (int i=0;i<VMHL_N;i++) VMHL_ResultVector[i]=1./double(VMHL_N);
 }
else
 {
 for (int i=0;i<VMHL_N;i++) VMHL_ResultVector[i]/=sum;
 }
}
//---------------------------------------------------------------------------
double MHL_ProbabilityOfTournamentSelection(double *Fitness, double *VMHL_ResultVector_Probability, int T, int VMHL_N)
{
/*
Функция вычисляет вероятности выбора индивидов из популяции с помощью турнирной селекции.
Входные параметры:
 Fitness - указатель на вектор значений целевой функции (не пригодности) индивидов;
 VMHL_ResultVector_Probability - указатель на вектор, в который будет проводиться запись;
 T - размер турнира;
 VMHL_N -  размер массивов.
Возвращаемое значение:
 Сумму вектора вероятностей Probability.
Примечание:
 Данная функция не нужна для работы турнирной селекции через функцию MHL_TournamentSelection
 в генетическом алгоритме. Функция предназначена для научных изысканий по исследованию работы
 различных видов селекций.
*/
    int i,j,minTn1,j0;
    double n1,n0,s,KC1,KC2,VMHL_Result;

    //пробегаем по всем индивидам
    for (i=0;i<VMHL_N;i++)
    {
        n0=0;//количество индивидов, которые выигрывают данный индивид
        n1=0;//количество индивидов, которые равны данному индивиду
        //Далее посчитаем их
        for (j=0;j<VMHL_N;j++) if (Fitness[j]>Fitness[i]) n0++;
        for (j=0;j<VMHL_N;j++) if (Fitness[j]==Fitness[i]) n1++;


        j0=TMHL_Max(1,int(T-(VMHL_N-n1-n0)));
        minTn1=TMHL_Min(T,int(n1));
        s=0;
        //пробегаем по всем допустимым сочетаниям
        //точная формула в справке
        for (j=j0-1;j<minTn1;j++)
        {
                KC1=TMHL_KCombinations(j+1,int(n1));// число возможных сочетаний
                KC2=TMHL_KCombinations(T-(j+1),int(VMHL_N-n1-n0));// число возможных сочетаний
                s+=KC1*KC2;
        }

        VMHL_ResultVector_Probability[i]=s/(n1*TMHL_KCombinations(T,VMHL_N));
    }


    VMHL_Result=TMHL_SumVector(VMHL_ResultVector_Probability,VMHL_N);
    return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_ProportionalSelection(double *Fitness, int VMHL_N)
{
/*
Пропорциональная селекция. Оператор генетического алгоритма. Работает с массивом пригодностей.
Входные параметры:
 Fitness - массив пригодностей (можно подавать не массив пригодностей, а массив значений целевой функции, но только для задач безусловной оптимизации);
 VMHL_N - размер массива пригодностей.
Возвращаемое значение:
 Номер выбранной пригодности, а, соответственно, номер индивида популяции.
Примечание:
 Использовать реализацию оператора ГА в виде этой функции нецелесообразно ввиду того, что при каждом запуске
 создается дополнительный массив.
 Данная функция аналогична по действию (результат действия аналогичен):
 1. Связке функций MHL_MakeVectorOfProbabilityForProportionalSelectionV2 и MHL_ProportionalSelectionV2
 2. Функции MHL_ProportionalSelectionV3
 Различия по временным затратам на выполнение. У этой реализации самое большое время выполнения.
*/
//Выбор в пропорциональной селекции производится согласно вектору вероятностей выбора индивидов.
//Создадим этот вектор
double *VectorOfProbability;
VectorOfProbability=new double[VMHL_N];

//Вектор Fitness мы не меняем. Поэтому проводим копирование.
TMHL_VectorToVector(Fitness,VectorOfProbability,VMHL_N);

//Проводим нормировку вектора, с целью получения вектора вероятностей.
//Нормировка вектора чисел такая, чтобы максимальный элемент имел значение 1, а минимальный 0.
//После данный нормализованный вектор сжимается так чтобы сумма всех элементов стала равна 1.
MHL_NormalizationVectorOne (VectorOfProbability,VMHL_N);

//Зная теперь вероятность выбора каждого индивида, проводим случайный выбор индивида.
int VMHL_Result=MHL_SelectItemOnProbability(VectorOfProbability,VMHL_N);

delete [] VectorOfProbability;

//Возвращаем номер выбранного индивида
return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_ProportionalSelectionV2(double *VectorOfProbability, int VMHL_N)
{
/*
Пропорциональная селекция. Оператор генетического алгоритма. Работает с вектором вероятностей выбора индивидов,
который можно получить из вектора пригодностей индивидов посредством функции MHL_MakeVectorOfProbabilityForProportionalSelectionV2.
Входные параметры:
 VectorOfProbability - массив вероятностей выбора индивидов для порпоциональной селекции;
 VMHL_N - размер массива пригодностей.
Возвращаемое значение:
 Номер выбранной пригодности, а, соответственно, номер индивида популяции.
Примечание:
 Связка данной функции и MHL_MakeVectorOfProbabilityForProportionalSelectionV2 аналогична по действию (результат действия аналогичен):
 1. Функции MHL_ProportionalSelection
 2. Функции MHL_ProportionalSelectionV3
 Различия по временным затратам на выполнение. У этой связки выполнение быстрее, чем у MHL_ProportionalSelection.
*/
int VMHL_Result=MHL_SelectItemOnProbability(VectorOfProbability,VMHL_N);
return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_ProportionalSelectionV3(double *Fitness, int VMHL_N)
{
/*
Пропорциональная селекция. Оператор генетического алгоритма. Работает с массивом пригодностей (обязательно не отрицательными).
Входные параметры:
 Fitness - массив пригодностей (В отличии от MHL_ProportionalSelection вектор пригодностей должен быть именно вектором пригодностей, то есть все элементы Fitness должны быть больше нуля);
 VMHL_N - размер массива пригодностей.
Возвращаемое значение:
 Номер выбранной пригодности, а, соответственно, номер индивида популяции.
Примечание:
 Данная функция аналогична по действию (результат действия аналогичен):
 1. Связке функций MHL_MakeVectorOfProbabilityForProportionalSelectionV2 и MHL_ProportionalSelectionV2
 2. Функции MHL_ProportionalSelection
 Различия по временным затратам на выполнение. Эта реализация быстрее, чем MHL_SelectionProportional
 и почти равна связке функций MHL_MakeVectorOfProbabilityForProportionalSelectionV2 и MHL_ProportionalSelectionV2,
 но реализация отличается от формульной записи в угоду более простой записи в программировании, но ей тождественна.
*/
int VMHL_Result=-1;//номер выбранного родителя
double Sum,r,s=0;
int i;
//получим сумму всех значений пригодностей
Sum=TMHL_SumVector(Fitness,VMHL_N);
r=MHL_RandomUniform(0,Sum);//случайное число по сумме
i=0;
while ((VMHL_Result==-1)&&(i<VMHL_N))
 {
 //определяем выбранного индивида
 s+=Fitness[i];
 if (s>=r) VMHL_Result=i;
 i++;
 }
return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_RankSelection(double *VectorOfProbability, int VMHL_N)
{
/*
Ранговая селекция. Оператор генетического алгоритма. Работает с вектором вероятностей выбора индивидов,
который можно получить из вектора пригодностей индивидов посредством функции MHL_MakeVectorOfRankForRankSelection
(для получения массива рангов) и потом функции MHL_MakeVectorOfProbabilityForProportionalSelectionV2
(для получения массива вероятностей выбора индивидов по рангам).
Входные параметры:
 VectorOfProbability - массив вероятностей выбора индивидов для ранговой селекции;
 VMHL_N - размер массива VectorProbability.
Возвращаемое значение:
 Номер выбранного индивида популяции.
*/
int VMHL_Result=MHL_SelectItemOnProbability(VectorOfProbability,VMHL_N);
return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_SelectItemOnProbability(double *P, int VMHL_N)
{
/*
Функция выбирает случайно номер элемента из вектора, где вероятность выбора каждого элемента
определяется значением в векторе P.
Входные параметры:
 P - вектор вероятностей выбора каждого элемента, то есть его компоненты должны быть из отрезка [0;1], а сумма их равна 1;
 VMHL_N - размер вектора.
Возвращаемое значение:
 Номер выбранного элемента.
Примечание:
 Проверка на правильность вектора P не проводится, так как функция обычно вызывается
 многократно, а проводить постоянно проверку накладно. Всё на Вашей совести.
*/
int i=0;
int VMHL_Result=-1;//номер выбранного элемента
double s=0;
double r;
r=MHL_RandomNumber();//случайное число
while ((VMHL_Result==-1)&&(i<VMHL_N))
 {
 //определяем выбранный элемент
 s+=P[i];
 if (s>r) VMHL_Result=i;
 i++;
 }
return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_StandartBinaryGeneticAlgorithm(int *Parameters, double (*FitnessFunction)(int*,int), int *VMHL_ResultVector, double *VMHL_Result)
{
/*
Стандартный генетический алгоритм на бинарных строках. Реализация алгоритма из документа "Генетический алгоритм. Стандарт. v.3.0".
https://github.com/Harrix/Standard-Genetic-Algorithm
Алгоритм оптимизации. Ищет максимум целевой функции FitnessFunction.
Входные параметры:
 Parameters - Вектор параметров генетического алгоритма. Каждый элемент обозначает свой параметр:
  [0] - длина бинарной хромосомы (определяется задачей оптимизации, что мы решаем);
  [1] - число вычислений целевой функции (CountOfFitness);
  [2] - тип селекции (TypeOfSel):
        0 - ProportionalSelection (Пропорциональная селекция);
        1 - RankSelection (Ранговая селекция);
        2 - TournamentSelection (Турнирная селекция).
  [3] - тип скрещивания (TypeOfCros):
        0 - SinglepointCrossover (Одноточечное скрещивание);
        1 - TwopointCrossover (Двуточечное скрещивание);
        2 - UniformCrossover (Равномерное скрещивание).
  [4] - тип мутации (TypeOfMutation):
        0 - Weak (Слабая мутация);
        1 - Averagen (Средняя мутация);
        2 - Strong (Сильная мутация).
  [5] - тип формирования нового поколения (TypeOfForm):
        0 - OnlyOffspringGenerationForming (Только потомки);
        1 - OnlyOffspringWithBestGenerationForming (Только потомки и копия лучшего индивида).
 FitnessFunction - указатель на целевую функцию (если решается задача условной оптимизации, то учет ограничений должен быть включен в эту функцию);
 VMHL_ResultVector - найденное решение (бинарный вектор);
 VMHL_Result - значение целевой функции в точке, определенной вектором VMHL_ResultVector.
Возвращаемое значение:
 1 - завершил работу без ошибок. Всё хорошо.
 0 - возникли при работе ошибки. Скорее всего в этом случае в VMHL_ResultVector и в VMHL_Result не содержится решение задачи.
Пример значений рабочего вектора Parameters:
 Parameters[0]=50;
 Parameters[1]=100*100;
 Parameters[2]=2;
 Parameters[3]=2;
 Parameters[4]=1;
 Parameters[5]=1;
*/
//Считываем из Parameters параметры алгоритма
int ChromosomeLength=Parameters[0];//Длина хромосомы
int CountOfFitness=Parameters[1];//Число вычислений целевой функции
int TypeOfSel=Parameters[2];//Тип селекции
int TypeOfCros=Parameters[3];//Тип скрещивания
int TypeOfMutation=Parameters[4];//Тип мутации
int TypeOfForm=Parameters[5];//Тип формирования нового поколения

//Проверим данные
if (ChromosomeLength<1)	return 0;//Слишком маленькая длина хромосомы
if (CountOfFitness<1)	return 0;//Слишком маленькое число вычислений целевой функции
if (!((TypeOfSel==0)||(TypeOfSel==1)||(TypeOfSel==2))) return 0;//Тип селекции указан не верно
if (!((TypeOfCros==0)||(TypeOfCros==1)||(TypeOfCros==2))) return 0;//Тип скрещивания указан не верно
if (!((TypeOfMutation==0)||(TypeOfMutation==1)||(TypeOfMutation==2))) return 0;//Тип мутации указан не верно
if (!((TypeOfForm==0)||(TypeOfForm==1))) return 0;//Тип формирования нового поколения указан не верно

//Теперь определим остальные параметры алгоритма, исходя из полученной информации
//Размер популяции и число поколений должны быть приблизительно равны (насколько это возможно)
int NumberOfGenerations=int(sqrt(double(CountOfFitness)));//Число поколений
int PopulationSize=int(CountOfFitness/NumberOfGenerations);//Размер популяции
int SizeOfTournament=2;//В стандартном генетическом алгоритме на бинарных строках размер турнира равен 2

//Переменные
int I,i,j;//Счетчики
int NumberOfMaximumFitness;//Номер лучшего индивида в текущей популяции
double MaximumFitness;//Значение целевой функции лучшего индивида в текущей популяции
double BestFitness;//Значение целевой функции лучшего индивида за всё время работы алгоритма
int NumberOfParent1;//Номер первого выбранного родителя
int NumberOfParent2;//Номер второго выбранного родителя
double ProbabilityOfMutation;//Вероятность мутации

//Для выполнения алгоритма требуются некоторые дополнительные массивы. Создадим их.
//Популяция индивидов
int **Population;
Population=new int*[PopulationSize];
for (i=0;i<PopulationSize;i++) Population[i]=new int[ChromosomeLength];
//Популяция потомков
int **ChildrenPopulation;
ChildrenPopulation=new int*[PopulationSize];
for (i=0;i<PopulationSize;i++) ChildrenPopulation[i]=new int[ChromosomeLength];
//Массив значений целевой функции индивидов
double *Fitness;
Fitness=new double[PopulationSize];
//Массив значений целевой функции потомков
double *ChildrenFitness;
ChildrenFitness=new double[PopulationSize];
//Массив для хранения произвольного индивида
int *TempIndividual;
TempIndividual=new int[ChromosomeLength];
//Массив для хранения лучшего индивида за всё время работы алгоритма
int *BestIndividual;
BestIndividual=new int[ChromosomeLength];
//Для пропорциональной и ранговой селекции нужен массив вероятностей выбора индивидов
double *VectorOfProbability;
VectorOfProbability=new double[PopulationSize];
//Для ранговой селекции нужен массив рангов индивидов
double *Rank;
Rank=new double[PopulationSize];
//Для турнирной селекции нужен служебный массив, содержащий информация о том, в турнире или нет индивид;
int *Taken;
Taken=new int[PopulationSize];
//Массив для хранения первого родителя
int *Parent1;
Parent1=new int[ChromosomeLength];
//Массив для хранения второго родителя
int *Parent2;
Parent2=new int[ChromosomeLength];
//Массив для хранения потомка от скрещивания двух родителей
int *Child;
Child=new int[ChromosomeLength];

//Инициализация начальной популяции
TMHL_RandomBinaryMatrix(Population,PopulationSize,ChromosomeLength);

//Вычислим значение целевой функции для каждого индивида
for (i=0;i<PopulationSize;i++)
 {
 //Копируем индивида во временного индивида, так как целевая функция работает с вектором, а не матрицей
 TMHL_MatrixToRow(Population,TempIndividual,i,ChromosomeLength);
 try
  {
  Fitness[i]=FitnessFunction(TempIndividual,ChromosomeLength);
  }
 catch(...)
  {
  return 0;//Генетический алгоритм не смог посчитать значение целевая функции индивида
  }
 }

//Определим наилучшего индивида и запомним его
NumberOfMaximumFitness=TMHL_NumberOfMaximumOfVector(Fitness,PopulationSize);
MaximumFitness=TMHL_MaximumOfVector(Fitness,PopulationSize);
TMHL_MatrixToRow(Population,BestIndividual,NumberOfMaximumFitness,ChromosomeLength);//Запоминаем индивида
BestFitness=MaximumFitness;//Запоминаем его значение целевой функции

for (I=1;I<NumberOfGenerations;I++)
 {//////////////////// ГЛАВНЫЙ ЦИКЛ ///////////////////////

 //Подготовка массивов для оператора селекции
 if (TypeOfSel==1)
  {
  //Для ранговой селекции нужен массив рангов индивидов
  MHL_MakeVectorOfRankForRankSelection(Fitness,Rank,PopulationSize);
  //Для ранговой селекции нужен массив вероятностей выбора индивидов из рангов
  MHL_MakeVectorOfProbabilityForRanklSelection(Rank,VectorOfProbability,PopulationSize);
  }
 if (TypeOfSel==0)//Для пропорциональной нужен массив вероятностей выбора индивидов
  MHL_MakeVectorOfProbabilityForProportionalSelectionV2(Fitness,VectorOfProbability,PopulationSize);

 for (j=0;j<PopulationSize;j++)
  {//Формирование популяции потомков
  if (TypeOfSel==0)//Пропорциональная селекция
   {
   //Выбираем двух родителей (точнее их номера)
   NumberOfParent1=MHL_ProportionalSelectionV2(VectorOfProbability,PopulationSize);
   NumberOfParent2=MHL_ProportionalSelectionV2(VectorOfProbability,PopulationSize);
   }
  if (TypeOfSel==1)//Ранговая селекция
   {
   //Выбираем двух родителей (точнее их номера)
   NumberOfParent1=MHL_RankSelection(VectorOfProbability,PopulationSize);
   NumberOfParent2=MHL_RankSelection(VectorOfProbability,PopulationSize);
   }
  if (TypeOfSel==2)//Турнирная селекция
   {
   //Выбираем двух родителей (точнее их номера)
   NumberOfParent1=MHL_TournamentSelection(Fitness,SizeOfTournament,Taken,PopulationSize);
   NumberOfParent2=MHL_TournamentSelection(Fitness,SizeOfTournament,Taken,PopulationSize);
   }

  //Копируем родителей из популяции
  TMHL_MatrixToRow(Population,Parent1,NumberOfParent1,ChromosomeLength);//Первого родителя
  TMHL_MatrixToRow(Population,Parent2,NumberOfParent2,ChromosomeLength);//Второго родителя

  //Теперь путем скрещивания получаем потомка
  if (TypeOfCros==0)//Одноточечное скрещивание
   TMHL_SinglepointCrossover(Parent1,Parent2,Child,ChromosomeLength);
  if (TypeOfCros==1)//Двухточечное скрещивание
   TMHL_TwopointCrossover(Parent1,Parent2,Child,ChromosomeLength);
  if (TypeOfCros==2)//Равномерное скрещивание
   TMHL_UniformCrossover(Parent1,Parent2,Child,ChromosomeLength);

  //Переместим потомка в массив потомков
  TMHL_RowToMatrix(ChildrenPopulation,Child,j,ChromosomeLength);
  }//Формирование популяции потомков

 //Мутируем получившуюся популяцию потомков
 //Но вначале определим вероятность мутации
 if (TypeOfMutation==0)//Слабая
  ProbabilityOfMutation=1./(3.*double(ChromosomeLength));
 if (TypeOfMutation==1)//Средняя
  ProbabilityOfMutation=1./double(ChromosomeLength);
 if (TypeOfMutation==2)//Сильняя
  ProbabilityOfMutation=TMHL_Min(3./double(ChromosomeLength),1.);
 TMHL_MutationBinaryMatrix(ChildrenPopulation,ProbabilityOfMutation,PopulationSize,ChromosomeLength);//Мутируем

 //Вычислим значение целевой функции для каждого потомка
 for (i=0;i<PopulationSize;i++)
  {
  //Копируем потомка во временного индивида, так как целевой функция работает с вектором, а не матрицей
  TMHL_MatrixToRow(ChildrenPopulation,TempIndividual,i,ChromosomeLength);
  try
   {
   ChildrenFitness[i]=FitnessFunction(TempIndividual,ChromosomeLength);
   }
  catch(...)
   {
   return 0;//Генетический алгоритм не смог посчитать значение целевой функции потомка
   }
  }

 //Определим наилучшего потомка и запомним его
 MaximumFitness=TMHL_MaximumOfVector(ChildrenFitness,PopulationSize);

 //Является ли лучшее решение на данном поколении лучше лучшего решения за всё время работы алгоритма
 if (MaximumFitness>BestFitness)
  {
  //Если всё-таки лучше
  NumberOfMaximumFitness=TMHL_NumberOfMaximumOfVector(ChildrenFitness,PopulationSize);
  TMHL_MatrixToRow(ChildrenPopulation,BestIndividual,NumberOfMaximumFitness,ChromosomeLength);//Запоминаем индивида
  BestFitness=MaximumFitness;//Запоминаем его значение целевой функции
  }

 //Теперь сформируем новое поколение
 if (TypeOfForm==0)//Только потомки
  {
  TMHL_MatrixToMatrix(ChildrenPopulation,Population,PopulationSize,ChromosomeLength);
  TMHL_VectorToVector(ChildrenFitness,Fitness,PopulationSize);
  }
 if (TypeOfForm==1)//Только потомки и копия лучшего индивида
  {
  TMHL_MatrixToMatrix(ChildrenPopulation,Population,PopulationSize,ChromosomeLength);
  TMHL_RowToMatrix(Population,BestIndividual,0,ChromosomeLength);
  TMHL_VectorToVector(ChildrenFitness,Fitness,PopulationSize);
  Fitness[0]=BestFitness;
  }

 }//////////////////// ГЛАВНЫЙ ЦИКЛ ///////////////////////

//Генетический алгоритм закончил свою работу
//Выдадим найденное лучшее решение за время запуска алгоритма и его значение целевой функции
TMHL_VectorToVector(BestIndividual,VMHL_ResultVector,ChromosomeLength);
*VMHL_Result=BestFitness;

//Удалим все дополнительные массивы
for (i=0;i<PopulationSize;i++) delete [] Population[i];
 delete [] Population;
for (i=0;i<PopulationSize;i++) delete [] ChildrenPopulation[i];
 delete [] ChildrenPopulation;
delete [] Fitness;
delete [] ChildrenFitness;
delete [] TempIndividual;
delete [] BestIndividual;
delete [] VectorOfProbability;
delete [] Rank;
delete [] Taken;
delete [] Parent1;
delete [] Parent2;
delete [] Child;

return 1;//Всё успешно
}
//---------------------------------------------------------------------------
int MHL_StandartGeneticAlgorithm(int *Parameters, double (*FitnessFunction)(int*,int), int *VMHL_ResultVector, double *VMHL_Result)
{
/*
Стандартный генетический алгоритм на бинарных строках. Реализация алгоритма из документа "Генетический алгоритм. Стандарт. v.3.0".
https://github.com/Harrix/Standard-Genetic-Algorithm
Алгоритм оптимизации. Ищет максимум целевой функции FitnessFunction.
Входные параметры:
 Parameters - Вектор параметров генетического алгоритма. Каждый элемент обозначает свой параметр:
  [0] - длина бинарной хромосомы (определяется задачей оптимизации, что мы решаем);
  [1] - число вычислений целевой функции (CountOfFitness);
  [2] - тип селекции (TypeOfSel):
        0 - ProportionalSelection (Пропорциональная селекция);
        1 - RankSelection (Ранговая селекция);
        2 - TournamentSelection (Турнирная селекция).
  [3] - тип скрещивания (TypeOfCros):
        0 - SinglepointCrossover (Одноточечное скрещивание);
        1 - TwopointCrossover (Двуточечное скрещивание);
        2 - UniformCrossover (Равномерное скрещивание).
  [4] - тип мутации (TypeOfMutation):
        0 - Weak (Слабая мутация);
        1 - Averagen (Средняя мутация);
        2 - Strong (Сильная мутация).
  [5] - тип формирования нового поколения (TypeOfForm):
        0 - OnlyOffspringGenerationForming (Только потомки);
        1 - OnlyOffspringWithBestGenerationForming (Только потомки и копия лучшего индивида).
 FitnessFunction - указатель на целевую функцию (если решается задача условной оптимизации, то учет ограничений должен быть включен в эту функцию);
 VMHL_ResultVector - найденное решение (бинарный вектор);
 VMHL_Result - значение целевой функции в точке, определенной вектором VMHL_ResultVector.
Возвращаемое значение:
 1 - завершил работу без ошибок. Всё хорошо.
 0 - возникли при работе ошибки. Скорее всего в этом случае в VMHL_ResultVector и в VMHL_Result не содержится решение задачи.
Пример значений рабочего вектора Parameters:
 Parameters[0]=50;
 Parameters[1]=100*100;
 Parameters[2]=2;
 Parameters[3]=2;
 Parameters[4]=1;
 Parameters[5]=1;
*/
int VMHL_Success;//Успешен ли будет запуск cГА

VMHL_Success=MHL_StandartBinaryGeneticAlgorithm(Parameters, FitnessFunction, VMHL_ResultVector, VMHL_Result);

return VMHL_Success;
}
//---------------------------------------------------------------------------
int MHL_StandartGeneticAlgorithm(int *Parameters, int *NumberOfParts, double *Left, double *Right, double (*FitnessFunction)(double*,int), double *VMHL_ResultVector, double *VMHL_Result)
{
/*
Стандартный генетический алгоритм на вещественных строках.
Реализация алгоритма из документа "Генетический алгоритм. Стандарт. v.3.0".
https://github.com/Harrix/Standard-Genetic-Algorithm
Алгоритм оптимизации. Ищет максимум целевой функции FitnessFunction.
Входные параметры:
 Parameters - Вектор параметров генетического алгоритма. Каждый элемент обозначает свой параметр:
  [0] - длина вещественной хромосомы (определяется задачей оптимизации, что мы решаем);
  [1] - число вычислений целевой функции (CountOfFitness);
  [2] - тип селекции (TypeOfSel):
        0 - ProportionalSelection (Пропорциональная селекция);
        1 - RankSelection (Ранговая селекция);
        2 - TournamentSelection (Турнирная селекция).
  [3] - тип скрещивания (TypeOfCros):
        0 - SinglepointCrossover (Одноточечное скрещивание);
        1 - TwopointCrossover (Двуточечное скрещивание);
        2 - UniformCrossover (Равномерное скрещивание).
  [4] - тип мутации (TypeOfMutation):
        0 - Weak (Слабая мутация);
        1 - Average (Средняя мутация);
        2 - Strong (Сильная мутация).
  [5] - тип формирования нового поколения (TypeOfForm):
        0 - OnlyOffspringGenerationForming (Только потомки);
        1 - OnlyOffspringWithBestGenerationForming (Только потомки и копия лучшего индивида)
  [6] - тип преобразования задачи вещественной оптимизации в задачу бинарной оптимизации (TypOfConverting);
        0 - IntConverting (Стандартное представление целого числа – номер узла в сетке дискретизации);
        1 - GrayСodeConverting (Стандартный рефлексивный Грей-код).
 NumberOfParts - указатель на массив: на сколько частей делить каждую вещественную координату при дискретизации (размерность Parameters[0]);
  Желательно брать по формуле NumberOfParts[i]=(2^k)-1, где k - натуральное число, например, 12.
 Left - массив левых границ изменения каждой вещественной координаты (размерность Parameters[0]);
 Right - массив правых границ изменения каждой вещественной координаты (размерность Parameters[0]);
 FitnessFunction - указатель на целевую функцию (если решается задача условной оптимизации, то учет ограничений должен быть включен в эту функцию);
 VMHL_ResultVector - найденное решение (вещественный вектор);
 VMHL_Result - значение целевой функции в точке, определенной вектором VMHL_ResultVector.
Возвращаемое значение:
 1 - завершил работу без ошибок. Всё хорошо.
 0 - возникли при работе ошибки. Скорее всего в этом случае в VMHL_ResultVector и в VMHL_Result не содержится решение задачи.
Пример значений рабочего вектора Parameters:
 Parameters[0]=2;
 Parameters[1]=100*100;
 Parameters[2]=2;
 Parameters[3]=2;
 Parameters[4]=1;
 Parameters[5]=1;
 Parameters[6]=0;
*/
int VMHL_Success;//Успешен ли будет запуск cГА

VMHL_Success=MHL_StandartRealGeneticAlgorithm(Parameters, NumberOfParts, Left, Right, FitnessFunction, VMHL_ResultVector, VMHL_Result);

return VMHL_Success;
}
//---------------------------------------------------------------------------
int MHL_StandartRealGeneticAlgorithm(int *Parameters, int *NumberOfParts, double *Left, double *Right, double (*FitnessFunction)(double*,int), double *VMHL_ResultVector, double *VMHL_Result)
{
/*
Стандартный генетический алгоритм на вещественных строках.
Реализация алгоритма из документа "Генетический алгоритм. Стандарт. v.3.0".
https://github.com/Harrix/Standard-Genetic-Algorithm
Алгоритм оптимизации. Ищет максимум целевой функции FitnessFunction.
Входные параметры:
 Parameters - Вектор параметров генетического алгоритма. Каждый элемент обозначает свой параметр:
  [0] - длина вещественной хромосомы (определяется задачей оптимизации, что мы решаем);
  [1] - число вычислений целевой функции (CountOfFitness);
  [2] - тип селекции (TypeOfSel):
        0 - ProportionalSelection (Пропорциональная селекция);
        1 - RankSelection (Ранговая селекция);
        2 - TournamentSelection (Турнирная селекция).
  [3] - тип скрещивания (TypeOfCros):
        0 - SinglepointCrossover (Одноточечное скрещивание);
        1 - TwopointCrossover (Двуточечное скрещивание);
        2 - UniformCrossover (Равномерное скрещивание).
  [4] - тип мутации (TypeOfMutation):
        0 - Weak (Слабая мутация);
        1 - Average (Средняя мутация);
        2 - Strong (Сильная мутация).
  [5] - тип формирования нового поколения (TypeOfForm):
        0 - OnlyOffspringGenerationForming (Только потомки);
        1 - OnlyOffspringWithBestGenerationForming (Только потомки и копия лучшего индивида)
  [6] - тип преобразования задачи вещественной оптимизации в задачу бинарной оптимизации (TypeOfConverting);
        0 - IntConverting (Стандартное представление целого числа – номер узла в сетке дискретизации);
        1 - GrayСodeConverting (Стандартный рефлексивный Грей-код).
 NumberOfParts - указатель на массив: на сколько частей делить каждую вещественную координату при дискретизации (размерность Parameters[0]);
  Желательно брать по формуле NumberOfParts[i]=(2^k)-1, где k - натуральное число, например, 12.
 Left - массив левых границ изменения каждой вещественной координаты (размерность Parameters[0]);
 Right - массив правых границ изменения каждой вещественной координаты (размерность Parameters[0]);
 FitnessFunction - указатель на целевую функцию (если решается задача условной оптимизации, то учет ограничений должен быть включен в эту функцию);
 VMHL_ResultVector - найденное решение (вещественный вектор);
 VMHL_Result - значение целевой функции в точке, определенной вектором VMHL_ResultVector.
Возвращаемое значение:
 1 - завершил работу без ошибок. Всё хорошо.
 0 - возникли при работе ошибки. Скорее всего в этом случае в VMHL_ResultVector и в VMHL_Result не содержится решение задачи.
Пример значений рабочего вектора Parameters:
 Parameters[0]=2;
 Parameters[1]=100*100;
 Parameters[2]=2;
 Parameters[3]=2;
 Parameters[4]=1;
 Parameters[5]=1;
 Parameters[6]=0;
*/
//Переменные
int i;//Счетчик
int ChromosomeLength;//Длина бинарной строки
int VMHL_Success;//Успешен ли будет запуск cГА на бинарных строках

//Считываем из Parameters необходимые параметры алгоритма
int RealLength=Parameters[0];//Размерность вещественного вектора
int TypOfConverting=Parameters[6];//Тип преобразования задачи в бинарную задачу оптимизацию

//Проверим данные
for (i=0;i<RealLength;i++) if (Left[i]>Right[i]) return 0;//Левая граница не может быть больше правой
for (i=0;i<RealLength;i++) if (NumberOfParts[i]<1) return 0;//На слишком мало частей предложено делить каждую координату
if (!((TypOfConverting==0)||(TypOfConverting==1))) return 0;//Тип преобразования указан не верно

//Для выполнения алгоритма требуются некоторые дополнительные массивы. Создадим их.
//Массив значений, сколько бит приходится в бинарной хромосоме на кодирование
//каждой вещественной координаты
int *Lengthi;
Lengthi=new int[RealLength];
//Параметры стандартного генетического алгоритма на бинарных строках
int *ParametersOfStandartBinaryGeneticAlgorithm;
//Указатель на массив, в который записываются решения при вычислении целевой функции
double *RealVector;
RealVector=new double[RealLength];

//Процесс преобразования задачи вещественной оптимизации в задачу бинарной оптимизации
//Определим длину бинарной хромосомы. При этом число точек для кодирования на одну больше, чем интервалов,
//на которые мы хотим разбить каждую вещественную координату.
for (int i=0;i<RealLength;i++)
 Lengthi[i]=MHL_HowManyPowersOfTwo(NumberOfParts[i]+1);

ChromosomeLength=TMHL_SumVector(Lengthi,RealLength);//Просуммируем элементы вектора

//Бинарное решение бинарной задачи оптимизации
int *BinaryDecision;
BinaryDecision=new int[ChromosomeLength];

//Создадим еще один массив для хранения бинарного массива для преобразования строки Грей-кода в бинарную
int *TempBinaryVector;
TempBinaryVector=new int[ChromosomeLength];//GrayСodeConverting (Стандартный рефлексивный Грей-код)

//Определим параметры стандартного генетического алгоритма на бинарных строках
ParametersOfStandartBinaryGeneticAlgorithm=new int[6];
ParametersOfStandartBinaryGeneticAlgorithm[0]=ChromosomeLength;//Длина хромосомы
ParametersOfStandartBinaryGeneticAlgorithm[1]=Parameters[1];//Число вычислений целевой функции
ParametersOfStandartBinaryGeneticAlgorithm[2]=Parameters[2];//Тип селекции
ParametersOfStandartBinaryGeneticAlgorithm[3]=Parameters[3];//Тип скрещивания
ParametersOfStandartBinaryGeneticAlgorithm[4]=Parameters[4];//Тип мутации
ParametersOfStandartBinaryGeneticAlgorithm[5]=Parameters[5];//Тип формирования нового поколения

//сГА на бинарных строках работает с функцией типа double (*)(int*,int)
//то есть в качестве входных параметров только решение и его размерность
//но для вычисления значения целевой функции еще требуются дополнительные переменные
//целевая функция для вещественного решения и так далее.
//Делаем их доступными, используя служебные дополнительные указатели библиотеки
VMHL_TempFunction=FitnessFunction;//указатель на целевая функция для вещественного решения
VMHL_TempInt1=Lengthi;//указатель на массив, сколько бит приходится в бинарной хромосоме на кодирование
VMHL_TempDouble1=Left;//указатель на массив левых границ изменения вещественной переменной
VMHL_TempDouble2=Right;//указатель на массив правых границ изменения вещественной переменной
VMHL_TempDouble3=RealVector;//указатель на массив, в котором можно сохранить вещественный индивид
//при его раскодировании из бинарной строки
VMHL_TempInt2=&RealLength;//указатель на размерность вещественного вектора
VMHL_TempInt3=&TypOfConverting;//указатель на тип преобразования
if (TypOfConverting==1)//GrayСodeConverting (Стандартный рефлексивный Грей-код)
 VMHL_TempInt4=TempBinaryVector;//массив для хранения бинарного массива для преобразования строки Грей-кода в бинарную

//Выполнение стандартного генетического алгоритма на бинарных строках
try
 {
 //Выполнение стандартного генетического алгоритма на бинарных строках
 VMHL_Success=MHL_StandartBinaryGeneticAlgorithm(ParametersOfStandartBinaryGeneticAlgorithm,MHL_BinaryFitnessFunction,BinaryDecision,VMHL_Result);
 }
catch(...)
 {
 return 0;//Генетический алгоритм на бинарных строках завершился с ошибками
 }

if (VMHL_Success==1)
 {
 //VMHL_Result уже записан и определен, а вот VMHL_ResultVector (конечное решение) еще нет
 //так как есть только бинарное решение, а не вещественное, которое нам и нужно

 //Преобразование бинарного решения в вещественное
 if (TypOfConverting==0)//IntConverting (Стандартное представление целого числа – номер узла в сетке дискретизации)
  MHL_BinaryVectorToRealVector(BinaryDecision,VMHL_ResultVector,VMHL_TempDouble1,VMHL_TempDouble2,VMHL_TempInt1,RealLength);
 if (TypOfConverting==1)//GrayСodeConverting (Стандартный рефлексивный Грей-код)
  MHL_BinaryGrayVectorToRealVector(BinaryDecision,ChromosomeLength,VMHL_ResultVector,VMHL_TempDouble1,VMHL_TempDouble2,VMHL_TempInt1,RealLength);
 }
else
 return 0;//Генетический алгоритм на бинарных строках завершился с ошибками, но не в результате генерирования исключений

//Удалим массивы для запуска стандартного генетического алгоритма на бинарных строках
delete [] ParametersOfStandartBinaryGeneticAlgorithm;
delete [] Lengthi;
delete [] BinaryDecision;
delete [] RealVector;
delete [] TempBinaryVector;

//Обнулим дополнительные указатели
VMHL_TempFunction=NULL;
VMHL_TempInt1=NULL;
VMHL_TempDouble1=NULL;
VMHL_TempDouble2=NULL;
VMHL_TempInt2=NULL;
VMHL_TempDouble3=NULL;
VMHL_TempInt3=NULL;
if (TypOfConverting==1)//GrayСodeConverting (Стандартный рефлексивный Грей-код)
 VMHL_TempInt4=NULL;

return 1;//Всё успешно
}
//---------------------------------------------------------------------------
int MHL_TournamentSelection(double *Fitness, int SizeTournament, int VMHL_N)
{
/*
Турнирная селекция. Оператор генетического алгоритма. Работает с массивом пригодностей индивидов.
Входные параметры:
 Fitness - массив пригодностей индивидов;
 SizeTournament - размер турнира;
 VMHL_N - размер массива.
Возвращаемое значение:
 Номер выбранного индивида популяции.
Примечание:
 Является стандартной реализацией турнирной селекции. Это турнирная селекция без возвращения.
*/
if (SizeTournament<2) SizeTournament=2;
if (SizeTournament>VMHL_N) SizeTournament=VMHL_N;

int j;//Счетчик
int p;//Текущее число свободнных участников
int r;//случайное число для определения победителя
int g=0;//Номер выбранного участника

int *Taken;//Информация о том, в турнире или нет индивид
Taken=new int[VMHL_N];
TMHL_ZeroVector(Taken,VMHL_N);// Пока никого

int VMHL_Result;//победитель (номер) для турнирной селекции
VMHL_Result=MHL_RandomUniformInt(0,VMHL_N);//первый участник
Taken[VMHL_Result]=1;//отметили первого участника

for (int i=1;i<SizeTournament;i++)
 {//выбор еще одного участника турнира
 r=MHL_RandomUniformInt(0,VMHL_N-i);//на один меньше можно выбрать, чем в предыдущий раз
 p=0;//Текущее число свободнных участников
 j=0;//Счетчик

 while (p!=r+1)
  {
  //Ищем нашего участника
  if (Taken[j]==0)
   {
   //Нашли свободного участника. Возможно это наш.
   p++;
   g=j;
   }
  j++;
  }

 //Теперь g - номер нашего участника
 Taken[g]=1;//Отметим

 // Выйграл ли новый участник лучшего представителя турнира
 if (Fitness[g]>Fitness[VMHL_Result]) VMHL_Result=g;
 }//выбор еще одного участника турнира

delete [] Taken;

return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_TournamentSelection(double *Fitness, int SizeTournament, int *Taken, int VMHL_N)
{
/*
Турнирная селекция. Оператор генетического алгоритма. Работает с массивом пригодностей индивидов.
Входные параметры:
 Fitness - массив пригодностей индивидов;
 SizeTournament - размер турнира
 Taken - Информация о том, в турнире или нет индивид (служебный массив);
 VMHL_N - размер массива.
Возвращаемое значение:
 Номер выбранного индивида популяции.
Примечание:
 Является стандартной реализацией турнирной селекции. Это турнирная селекция без возвращения.
Примечание:
 В работе функции используется вспомогательный массив Taken. Так как функция вызывается часто, то
каждый раз создавать массив затратно. Поэтому можно в перегруженной функции передать этот массив
в качестве параметра.
*/
if (SizeTournament<2) SizeTournament=2;
if (SizeTournament>VMHL_N) SizeTournament=VMHL_N;

int j;//Счетчик
int p;//Текущее число свободнных участников
int r;//случайное число для определения победителя
int g=0;//Номер выбранного участника

TMHL_ZeroVector(Taken,VMHL_N);// Пока никого

int VMHL_Result;//победитель (номер) для турнирной селекции
VMHL_Result=MHL_RandomUniformInt(0,VMHL_N);//первый участник
Taken[VMHL_Result]=1;//отметили первого участника

for (int i=1;i<SizeTournament;i++)
 {//выбор еще одного участника турнира
 r=MHL_RandomUniformInt(0,VMHL_N-i);//на один меньше можно выбрать, чем в предыдущий раз
 p=0;//Текущее число свободнных участников
 j=0;//Счетчик

 while (p!=r+1)
  {
  //Ищем нашего участника
  if (Taken[j]==0)
   {
   //Нашли свободного участника. Возможно это наш.
   p++;
   g=j;
   }
  j++;
  }

 //Теперь g - номер нашего участника
 Taken[g]=1;//Отметим

 // Выйграл ли новый участник лучшего представителя турнира
 if (Fitness[g]>Fitness[VMHL_Result]) VMHL_Result=g;
 }//выбор еще одного участника турнира

return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_TournamentSelectionWithReturn(double *Fitness, int SizeTournament, int VMHL_N)
{
/*
Турнирная селекция с возвращением. Оператор генетического алгоритма. Работает с массивом пригодностей индивидов.
Входные параметры:
 Fitness - массив пригодностей индивидов;
 VMHL_N - размер массива VectorProbability;
 SizeTournament - размер турнира.
Возвращаемое значение:
 Номер выбранного индивида популяции.
Примечание:
 Не является стандартной реализацией турнирной селекции, так как в классичсекой турнирной селекции
в один туринир один и тот же индивид может попасть только один раз.
*/
if (SizeTournament<2) SizeTournament=2;
if (SizeTournament>VMHL_N) SizeTournament=VMHL_N;
int VMHL_Result;//победитель (номер) для турнирной селекции
int r; //случайное число для определения победителя

//турнирная  селекция с возвращением.
VMHL_Result=MHL_RandomUniformInt(0,VMHL_N);//первый участник

for (int i=1;i<SizeTournament;i++)
 {//выбор еще одного участника турнира
 r=MHL_RandomUniformInt(0,VMHL_N);
 if (Fitness[r]>Fitness[VMHL_Result]) VMHL_Result=r;
 }//выбор еще одного участника турнира

return VMHL_Result;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Геометрия
//*****************************************************************

//*****************************************************************
//Гиперболические функции
//*****************************************************************
double MHL_Cosech(double x)
{
/*
Функция возвращает гиперболический косеканс.
Входные параметры:
 x - входная переменная.
Возвращаемое значение:
 Гиперболический косеканс.
*/
return 2./(exp(x)-exp(-x));
}
//---------------------------------------------------------------------------
double MHL_Cosh(double x)
{
/*
Функция возвращает гиперболический косинус.
Входные параметры:
 x - входная переменная.
Возвращаемое значение:
 Гиперболический косинус.
*/
return (exp(x)+exp(-x))/2.;
}
//---------------------------------------------------------------------------
double MHL_Cotanh(double x)
{
/*
Функция возвращает гиперболический котангенс.
Входные параметры:
 x - входная переменная.
Возвращаемое значение:
 Гиперболический котангенс.
*/
return (exp(x)+exp(-x))/(exp(x)-exp(-x));
}
//---------------------------------------------------------------------------
double MHL_Sech(double x)
{
/*
Функция возвращает гиперболический секанс.
Входные параметры:
 x - входная переменная.
Возвращаемое значение:
 Гиперболический секанс.
*/
return 2./(exp(x)+exp(-x));
}
//---------------------------------------------------------------------------
double MHL_Sinh(double x)
{
/*
Функция возвращает гиперболический синус.
Входные параметры:
 x - входная переменная.
Возвращаемое значение:
 Гиперболический синус.
*/
return (exp(x)-exp(-x))/2.;
}
//---------------------------------------------------------------------------
double MHL_Tanh(double x)
{
/*
Функция возвращает гиперболический тангенс.
Входные параметры:
 x - входная переменная.
Возвращаемое значение:
 Гиперболический тангенс.
*/
return (exp(x)-exp(-x))/(exp(x)+exp(-x));
}

//---------------------------------------------------------------------------

//*****************************************************************
//Дифференцирование
//*****************************************************************
double MHL_CenterDerivative(double x, double h, double (*Function)(double))
{
/*
Численное значение производной в точке (центральной разностной производной с шагом 2h).
Входные параметры:
 x - точка, в которой считается производная;
 h - малое приращение x;
 Function - функция, производная которой ищется.
Возвращаемое значение:
 Значение производной в точке.
Примечание:
 При h<=0 возвращается 0.
*/
if (h<=0) return 0;

return ((Function(x+h)-Function(x-h))/(2*h));
}
//---------------------------------------------------------------------------
double MHL_LeftDerivative(double x, double h, double (*Function)(double))
{
/*
Численное значение производной в точке (разностная производная влево).
Входные параметры:
 x - точка, в которой считается производная;
 h - малое приращение x;
 Function - функция, производная которой ищется.
Возвращаемое значение:
 Значение производной в точке.
Примечание:
 При h<=0 возвращается 0.
*/
if (h<=0) return 0;

return ((Function(x)-Function(x-h))/h);
}
//---------------------------------------------------------------------------
double MHL_RightDerivative(double x, double h, double (*Function)(double))
{
/*
Численное значение производной в точке (разностная производная вправо).
Входные параметры:
 x - точка, в которой считается производная;
 h - малое приращение x;
 Function - функция, производная которой ищется.
Возвращаемое значение:
 Значение производной в точке.
Примечание:
 При h<=0 возвращается 0.
*/
if (h<=0) return 0;

return ((Function(x+h)-Function(x))/h);
}
//---------------------------------------------------------------------------

//*****************************************************************
//Для тестовых функций
//*****************************************************************
int MHL_ClassOfTestFunction(TypeOfTestFunction Type)
{
    /*
    Функция выдает принадлежность тестовой функции к классу функций: бинарной, вещественной или иной оптимизации.
    Входные параметры:
     Type - тип тестовой функции.
     Смотреть виды в переменных перечисляемого типа в начале HarrixMathLibrary.h файла.
    Возвращаемое значение:
     Класс тестовой функции:
      1 - бинарной оптимизации;
      2 - вещественной оптимизации.
    */
    int VMHL_Result=0;

    if (Type==TestFunction_SumVector)
        return 1;

    if (Type==TestFunction_Ackley)
        return 2;

    if (Type==TestFunction_ParaboloidOfRevolution)
        return 2;

    if (Type==TestFunction_Rastrigin)
        return 2;

    if (Type==TestFunction_Rosenbrock)
        return 2;

    return VMHL_Result;
}
//---------------------------------------------------------------------------
void MHL_DefineTestFunction(TypeOfTestFunction Type)
{
    /*
    Служебная функция определяет тестовую функцию для других функций по работе с тестовыми функциями.
	Вызывать всегда, когда хотите вызвать функции по работе с тестовыми функциями (из одноименного раздела).
    Ответ представляет собой два действительных числа.
    Входные параметры:
     Type - обозначение тестовой функции, которую вызываем.
     Смотреть виды в переменных перечисляемого типа в начале HarrixMathLibrary.h файла.
    Возвращаемое значение:
     Отсутствует.
    */
    VMHL_TypeOfTestFunction=Type;
}
//---------------------------------------------------------------------------
double MHL_ErrorExOfTestFunction_Binary(int *x, int VMHL_N)
{
    /*
    Функция определяет значение ошибки по входным параметрам найденного решения
    в задаче оптимизации для тестовой функции. 
	Включает в себя все тестовые функции бинарной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     x - указатель на исходный массив (найденное решение алгоритмом);
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение ошибки по входным параметрам Ex.
    */
    double VMHL_Result_Ex = 0;
    int i;

    int *Optimum=new int[VMHL_N];

    MHL_OptimumOfTestFunction_Binary(Optimum, VMHL_N);


    if (VMHL_TypeOfTestFunction==TestFunction_SumVector)
    {
        for (i=0;i<VMHL_N;i++)
            VMHL_Result_Ex+=fabs(x[i]-Optimum[i]);
    }

    delete [] Optimum;

    return VMHL_Result_Ex/double(VMHL_N);
}
//---------------------------------------------------------------------------
double MHL_ErrorExOfTestFunction_Binary(int *x, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет значение ошибки по входным параметрам найденного решения
    в задаче оптимизации для тестовой функции. 
	Включает в себя все тестовые функции бинарной оптимизации.
    Входные параметры:
     x - указатель на исходный массив (найденное решение алгоритмом);
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Значение ошибки по входным параметрам Ex.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_ErrorExOfTestFunction_Binary(x, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_ErrorExOfTestFunction_Real(double *x, int VMHL_N)
{
    /*
    Функция определяет значение ошибки по входным параметрам найденного решения
    в задаче оптимизации для тестовой функции вещественной оптимизации.
	Включает в себя все тестовые функции вещественной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     x - указатель на исходный массив (найденное решение алгоритмом);
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение ошибки по входным параметрам Ex.
    */
    double VMHL_Result_Ex = 0;
    int i;

    double *Optimum=new double[VMHL_N];

    MHL_OptimumOfTestFunction_Real(Optimum, VMHL_N);

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
    {
        for (i=0;i<VMHL_N;i++)
            VMHL_Result_Ex+=(x[i]-Optimum[i])*(x[i]-Optimum[i]);
        VMHL_Result_Ex=sqrt(VMHL_Result_Ex)/double(VMHL_N);
    }

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
    {
        for (i=0;i<VMHL_N;i++)
            VMHL_Result_Ex+=(x[i]-Optimum[i])*(x[i]-Optimum[i]);
        VMHL_Result_Ex=sqrt(VMHL_Result_Ex)/double(VMHL_N);
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
    {
        for (i=0;i<VMHL_N;i++)
            VMHL_Result_Ex+=(x[i]-Optimum[i])*(x[i]-Optimum[i]);
        VMHL_Result_Ex=sqrt(VMHL_Result_Ex)/double(VMHL_N);
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
    {
        for (i=0;i<VMHL_N;i++)
            VMHL_Result_Ex+=(x[i]-Optimum[i])*(x[i]-Optimum[i]);
        VMHL_Result_Ex=sqrt(VMHL_Result_Ex)/double(VMHL_N);
    }

    delete [] Optimum;

    return VMHL_Result_Ex;
}
//---------------------------------------------------------------------------
double MHL_ErrorExOfTestFunction_Real(double *x, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет значение ошибки по входным параметрам найденного решения
    в задаче оптимизации для тестовой функции вещественной оптимизации.
	Включает в себя все тестовые функции вещественной оптимизации.
    Входные параметры:
     x - указатель на исходный массив (найденное решение алгоритмом);
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Значение ошибки по входным параметрам Ex.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_ErrorExOfTestFunction_Real(x, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_ErrorEyOfTestFunction_Binary(double FitnessOfx, int VMHL_N)
{
    /*
    Функция определяет значение ошибки по значениям целевой функции найденного решения
    в задаче оптимизации для тестовой функции. 
	Включает в себя все тестовые функции бинарной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     FitnessOfx - значение целевой функции найденного решения алгоритмом оптимизации;
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение ошибки по значениям целевой функции Ey.
    */
    double VMHL_Result_Ey = 0;

    double FitnessOfOptimum=MHL_FitnessOfOptimumOfTestFunction_Binary(VMHL_N);

    if (VMHL_TypeOfTestFunction==TestFunction_SumVector)
    {
        VMHL_Result_Ey=fabs(MHL_MaximumOrMinimumOfTestFunction_Binary()*FitnessOfx-FitnessOfOptimum);
		VMHL_Result_Ey/=double(VMHL_N);
    }

    return VMHL_Result_Ey;
}
//---------------------------------------------------------------------------
double MHL_ErrorEyOfTestFunction_Binary(double FitnessOfx, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет значение ошибки по значениям целевой функции найденного решения
    в задаче оптимизации для тестовой функции. 
	Включает в себя все тестовые функции бинарной оптимизации.
    Входные параметры:
     FitnessOfx - значение целевой функции найденного решения алгоритмом оптимизации;
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Значение ошибки по значениям целевой функции Ey.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_ErrorEyOfTestFunction_Binary(FitnessOfx, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_ErrorEyOfTestFunction_Real(double FitnessOfx, int VMHL_N)
{
    /*
    Функция определяет значение ошибки по значениям целевой функции найденного решения
    в задаче оптимизации для тестовой функции вещественной оптимизации.
	Включает в себя все тестовые функции вещественной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     FitnessOfx - значение целевой функции найденного решения алгоритмом оптимизации;
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение ошибки по значениям целевой функции Ey.
    */
    double VMHL_Result_Ey = 0;

    double FitnessOfOptimum=MHL_FitnessOfOptimumOfTestFunction_Real(VMHL_N);

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
    {
        VMHL_Result_Ey=fabs(MHL_MaximumOrMinimumOfTestFunction_Real()*FitnessOfx-FitnessOfOptimum);
    }

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
    {
        VMHL_Result_Ey=fabs(MHL_MaximumOrMinimumOfTestFunction_Real()*FitnessOfx-FitnessOfOptimum);
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
    {
        VMHL_Result_Ey=fabs(MHL_MaximumOrMinimumOfTestFunction_Real()*FitnessOfx-FitnessOfOptimum);
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
    {
        VMHL_Result_Ey=fabs(MHL_MaximumOrMinimumOfTestFunction_Real()*FitnessOfx-FitnessOfOptimum);
    }

    return VMHL_Result_Ey;
}
//---------------------------------------------------------------------------
double MHL_ErrorEyOfTestFunction_Real(double FitnessOfx, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет значение ошибки по значениям целевой функции найденного решения
    в задаче оптимизации для тестовой функции функции вещественной оптимизации.
    Включает в себя все тестовые функции вещественной оптимизации.
    Входные параметры:
     FitnessOfx - значение целевой функции найденного решения алгоритмом оптимизации;
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Значение ошибки по значениям целевой функции Ey.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_ErrorEyOfTestFunction_Real(FitnessOfx, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_ErrorROfTestFunction_Binary(int *x, int VMHL_N)
{
    /*
    Функция определяет значение надежности найденного решения
	в задаче оптимизации для тестовой функции бинарной оптимизации. 
	Включает в себя все тестовые функции бинарной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     x - указатель на исходный массив (найденное решение алгоритмом);
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение надежности R.
    */
    double VMHL_Result_R = 1;
    int i;

    int *Optimum=new int[VMHL_N];

    MHL_OptimumOfTestFunction_Binary(Optimum, VMHL_N);

    if (VMHL_TypeOfTestFunction==TestFunction_SumVector)
    {
        for (i=0;i<VMHL_N;i++)
        {
            if (x[i]!=Optimum[i]) VMHL_Result_R=0;
        }
    }

    delete [] Optimum;

    return VMHL_Result_R;
}
//---------------------------------------------------------------------------
double MHL_ErrorROfTestFunction_Binary(int *x, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет значение надежности найденного решения
    в задаче оптимизации для тестовой функции бинарной оптимизации. 
	Включает в себя все тестовые функции бинарной оптимизации.
    Входные параметры:
     x - указатель на исходный массив (найденное решение алгоритмом);
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Значение надежности R.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_ErrorROfTestFunction_Binary(x, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_ErrorROfTestFunction_Real(double *x, int VMHL_N)
{
    /*
    Функция определяет значение надежности найденного решения
    в задаче оптимизации для тестовой функции вещественной оптимизации. 
	Включает в себя все тестовые функции вещественной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     x - указатель на исходный массив (найденное решение алгоритмом);
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение надежности R.
    */
    double VMHL_Result_R = 1;
    int i;

    double *Optimum=new double[VMHL_N];

    MHL_OptimumOfTestFunction_Real(Optimum, VMHL_N);

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
    {
        for (i=0;i<VMHL_N;i++)
        {
            if (fabs(x[i]-Optimum[i])>=MHL_PrecisionOfCalculationsOfTestFunction_Real()) VMHL_Result_R=0;
        }
    }

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
    {
        for (i=0;i<VMHL_N;i++)
        {
            if (fabs(x[i]-Optimum[i])>=MHL_PrecisionOfCalculationsOfTestFunction_Real()) VMHL_Result_R=0;
        }
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
    {
        for (i=0;i<VMHL_N;i++)
        {
            if (fabs(x[i]-Optimum[i])>=MHL_PrecisionOfCalculationsOfTestFunction_Real()) VMHL_Result_R=0;
        }
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
    {
        for (i=0;i<VMHL_N;i++)
        {
            if (fabs(x[i]-Optimum[i])>=MHL_PrecisionOfCalculationsOfTestFunction_Real()) VMHL_Result_R=0;
        }
    }

    delete [] Optimum;

    return VMHL_Result_R;
}
//---------------------------------------------------------------------------
double MHL_ErrorROfTestFunction_Real(double *x, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет значение надежности найденного решения
    в задаче оптимизации для тестовой функции вещественной оптимизации. 
	Включает в себя все тестовые функции вещественной оптимизации.
    Входные параметры:
     x - указатель на исходный массив (найденное решение алгоритмом);
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Значение надежности R.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_ErrorROfTestFunction_Real(x, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_FitnessOfOptimumOfTestFunction_Binary(int VMHL_N)
{
    /*
    Функция определяет значение целевой функции в оптимуме для тестовой функции бинарной оптимизации.
	Включает в себя все тестовые функции бинарной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     VMHL_N - размер массива x в решаемой задаче оптимизации.
    Возвращаемое значение:
     Значение тестовой функции в оптимальной точке.
    */
    double VMHL_Result = 0;

    if (VMHL_TypeOfTestFunction==TestFunction_SumVector)
    {
        VMHL_Result = VMHL_N;
    }

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_FitnessOfOptimumOfTestFunction_Binary(int VMHL_N, TypeOfTestFunction Type)
{
    /*
	Функция определяет значение целевой функции в оптимуме для тестовой функции бинарной оптимизации.
    Включает в себя все тестовые функции бинарной оптимизации.
    Входные параметры:
     VMHL_N - размер массива x в решаемой задаче оптимизации;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Значение тестовой функции в оптимальной точке.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_FitnessOfOptimumOfTestFunction_Binary(VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_FitnessOfOptimumOfTestFunction_Real(double VMHL_N)
{
    /*
    Функция определяет значение целевой функции в оптимуме для тестовой функции вещественной оптимизации.
    Включает в себя все тестовые функции вещественной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     VMHL_N - размер массива x в решаемой задаче оптимизации.
    Возвращаемое значение:
     Значение тестовой функции в оптимальной точке.
    */
    double VMHL_Result = 0;

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
    {
        VMHL_Result = 0;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
    {
        VMHL_Result = 0;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
    {
        VMHL_Result = 0;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
    {
        VMHL_Result = 0;
    }

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_FitnessOfOptimumOfTestFunction_Real(double VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет значение целевой функции в оптимуме для тестовой функции вещественной оптимизации.
    Включает в себя все тестовые функции вещественной оптимизации.
    Входные параметры:
     VMHL_N - размер массива x в решаемой задаче оптимизации;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Значение тестовой функции в оптимальной точке.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_FitnessOfOptimumOfTestFunction_Real(VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_GetCountOfFitness()
{
    /*
    Функция выдает количество вызовов целевой функции.
    Вызывать после вызова алгорита оптимизации.
    Обязательно вызвать MHL_SetToZeroCountOfFitness один раз перед вызовом алгоритмов оптимизации
    при исследовании эффективности алгоритмов оптимизации, где требуется контроль числа вызовов целевой функции.
    Входные параметры:
     Отсутствуют.
    Возвращаемое значение:
     Количество вызовов целевой функции.
    */
    return CountOfFitness;
}
//---------------------------------------------------------------------------
void MHL_LeftAndRightBorderOfTestFunction_Real(double *Left, double *Right,int VMHL_N)
{
    /*
    Функция определяет левые и правые границы допустимой области для тестовой функции вещественной оптимизации.
    Более точную информацию ищите в описаниях тестовых функций:
    https://github.com/Harrix/HarrixTestFunctions
    Включает в себя все тестовые функции вещественной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     Left - указатель на массив, куда будет записываться результат левых границ допустимой области;
     Right - указатель на массив, куда будет записываться результат левых границ допустимой области;
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Отсутствует.
    */
    int i;

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
    {
        for (i=0;i<VMHL_N;i++) Left[i]=-5;
        for (i=0;i<VMHL_N;i++) Right[i]=5;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
    {
        for (i=0;i<VMHL_N;i++) Left[i]=-2;
        for (i=0;i<VMHL_N;i++) Right[i]=2;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
    {
        for (i=0;i<VMHL_N;i++) Left[i]=-5;
        for (i=0;i<VMHL_N;i++) Right[i]=5;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
    {
        for (i=0;i<VMHL_N;i++) Left[i]=-2;
        for (i=0;i<VMHL_N;i++) Right[i]=2;
    }
}
//---------------------------------------------------------------------------
void MHL_LeftAndRightBorderOfTestFunction_Real(double *Left, double *Right, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет левые и правые границы допустимой области для тестовой функции вещественной оптимизации.
    Более точную информацию ищите в описаниях тестовых функций:
    https://github.com/Harrix/HarrixTestFunctions
    Включает в себя все тестовые функции вещественной оптимизации.
    Входные параметры:
     NumberOfParts - указатель на массив, куда будет записываться результат;
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Точность вычислений.
    */
    VMHL_TypeOfTestFunction = Type;

    MHL_LeftAndRightBorderOfTestFunction_Real(Left, Right, VMHL_N);
}
//---------------------------------------------------------------------------
double MHL_MaximumOrMinimumOfTestFunction_Binary()
{
    /*
    Функция сообщает - ищется максимум или минимум в задаче оптимизации
    для тестовой функции бинарной оптимизации.
    Требуется для использования в других функций.
    Включает в себя все тестовые функции бинарной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     Отсутствуют.
    Возвращаемое значение:
     1 - задача на нахождение максимума;
     -1 - задача на нахождение минимума.
    */
    double VMHL_Result = 0;

    if (VMHL_TypeOfTestFunction==TestFunction_SumVector)
    {
        VMHL_Result=1;
    }

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_MaximumOrMinimumOfTestFunction_Binary(TypeOfTestFunction Type)
{
    /*
    Функция сообщает - ищется максимум или минимум в задаче оптимизации
    для тестовой функции бинарной оптимизации.
    Требуется для использования в других функций.
    Включает в себя все тестовые функции бинарной оптимизации.
    Входные параметры:
     Type - тип тестовой функции.
    Возвращаемое значение:
     Точность вычислений.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_MaximumOrMinimumOfTestFunction_Binary();

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_MaximumOrMinimumOfTestFunction_Real()
{
    /*
    Функция сообщает - ищется максимум или минимум в задаче оптимизации
    для тестовой функции вещественной оптимизации.
    Требуется для использования в других функций.
    Включает в себя все тестовые функции вещественной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     Отсутствуют.
    Возвращаемое значение:
     1 - задача на нахождение максимума;
     -1 - задача на нахождение минимума.
    */
    double VMHL_Result = 0;

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
    {
        VMHL_Result=-1;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
    {
        VMHL_Result=-1;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
    {
        VMHL_Result=-1;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
    {
        VMHL_Result=-1;
    }

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_MaximumOrMinimumOfTestFunction_Real(TypeOfTestFunction Type)
{
    /*
    Функция сообщает - ищется максимум или минимум в задаче оптимизации
    для тестовой функции вещественной оптимизации.
    Требуется для использования в других функций.
    Включает в себя все тестовые функции вещественной оптимизации.
    Входные параметры:
     Type - тип тестовой функции.
    Возвращаемое значение:
     Точность вычислений.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_MaximumOrMinimumOfTestFunction_Real();

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_NumberOfPartsOfTestFunction_Real(int *NumberOfParts, int VMHL_N)
{
    /*
    Функция определяет на сколько частей нужно делить каждую координату в задаче оптимизации
    для тестовой функции вещественной оптимизации для алгоритма дискретной оптимизации и какая при этом требуется
    точность для подсчета надежности. Более точную информацию ищите в описаниях тестовых функций:
    https://github.com/Harrix/HarrixTestFunctions
    Включает в себя все тестовые функции вещественной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     NumberOfParts - указатель на массив, куда будет записываться результат;
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Точность вычислений.
    */
    double VMHL_Result_E = 0;
    int i;

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
    {
        for (i=0;i<VMHL_N;i++) NumberOfParts[i]=4095;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
    {
        for (i=0;i<VMHL_N;i++) NumberOfParts[i]=4095;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
    {
        for (i=0;i<VMHL_N;i++) NumberOfParts[i]=4095;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
    {
        for (i=0;i<VMHL_N;i++) NumberOfParts[i]=4095;
    }

    VMHL_Result_E=MHL_PrecisionOfCalculationsOfTestFunction_Real();

    return VMHL_Result_E;
}
//---------------------------------------------------------------------------
double MHL_NumberOfPartsOfTestFunction_Real(int *NumberOfParts, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет на сколько частей нужно делить каждую координату в задаче оптимизации
    для тестовой функции вещественной оптимизации для алгоритма дискретной оптимизации и какая при этом требуется
    точность для подсчета надежности. Более точную информацию ищите в описаниях тестовых функций:
    https://github.com/Harrix/HarrixTestFunctions
    Включает в себя все тестовые функции вещественной оптимизации.
    Входные параметры:
     NumberOfParts - указатель на массив, куда будет записываться результат;
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
    Возвращаемое значение:
     Точность вычислений.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_NumberOfPartsOfTestFunction_Real(NumberOfParts, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_OptimumOfTestFunction_Binary(int *Optimum, int VMHL_N)
{
    /*
	Функция определяет значение оптимума для тестовой функции бинарной оптимизации.
	Включает в себя все тестовые функции бинарной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     Optimum - указатель на исходный массив, куда будет записываться результат;
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение тестовой функции в оптимальной точке.
    */
    double VMHL_Result = 0;
    int i;

    if (VMHL_TypeOfTestFunction==TestFunction_SumVector)
    {
        for (i=0;i<VMHL_N;i++)
            Optimum[i]=1;
        VMHL_Result = MHL_FitnessOfOptimumOfTestFunction_Binary(VMHL_N);
    }

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_OptimumOfTestFunction_Binary(int *Optimum, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет значение оптимума для тестовой функции бинарной оптимизации. 
	Включает в себя все тестовые функции бинарной оптимизации.
    Входные параметры:
     Optimum - указатель на исходный массив, куда будет записываться результат;
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
     Смотреть виды в переменных перечисляемого типа в начале HarrixMathLibrary.h файла.
    Возвращаемое значение:
     Значение тестовой функции в оптимальной точке.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_TestFunction_Binary(Optimum, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_OptimumOfTestFunction_Real(double *Optimum, int VMHL_N)
{
    /*
    Функция определяет значение оптимума для тестовой функции вещественной оптимизации.
    Включает в себя все тестовые функции вещественной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     Optimum - указатель на исходный массив, куда будет записываться результат;
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение тестовой функции в оптимальной точке.
    */
    double VMHL_Result = 0;
    int i;

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
    {
        for (i=0;i<VMHL_N;i++) Optimum[i]=0;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
    {
        for (i=0;i<VMHL_N;i++) Optimum[i]=0;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
    {
        for (i=0;i<VMHL_N;i++) Optimum[i]=0;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
    {
        for (i=0;i<VMHL_N;i++) Optimum[i]=1;
    }

    VMHL_Result = MHL_FitnessOfOptimumOfTestFunction_Real(VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_OptimumOfTestFunction_Real(double *Optimum, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Функция определяет значение оптимума для тестовой функции вещественной оптимизации.
    Включает в себя все тестовые функции вещественной оптимизации.
    Входные параметры:
     Optimum - указатель на исходный массив, куда будет записываться результат;
     VMHL_N - размер массива x;
     Type - тип тестовой функции.
     Смотреть виды в переменных перечисляемого типа в начале HarrixMathLibrary.h файла.
    Возвращаемое значение:
     Значение тестовой функции в оптимальной точке.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_OptimumOfTestFunction_Real(Optimum, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_PrecisionOfCalculationsOfTestFunction_Real()
{
    /*
    Функция определяет точность для подсчета надежности в задаче оптимизации
    для тестовой функции вещественной оптимизации для алгоритма дискретной оптимизации.
    Более точную информацию ищите в описаниях тестовых функций:
    https://github.com/Harrix/HarrixTestFunctions
	Включает в себя все тестовые функции вещественной оптимизации.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     Отсутствуют.
    Возвращаемое значение:
     Точность вычислений.
    */
    double VMHL_Result_E = 0;

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
    {
        VMHL_Result_E=0.025;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
    {
        VMHL_Result_E=0.01;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
    {
        VMHL_Result_E=0.025;
    }

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
    {
        VMHL_Result_E=0.01;
    }

    return VMHL_Result_E;
}
//---------------------------------------------------------------------------
double MHL_PrecisionOfCalculationsOfTestFunction_Real(TypeOfTestFunction Type)
{
    /*
    Функция определяет точность для подсчета надежности в задаче оптимизации
    для тестовой функции вещественной оптимизации для алгоритма дискретной оптимизации.
    Более точную информацию ищите в описаниях тестовых функций:
    https://github.com/Harrix/HarrixTestFunctions
    Включает в себя все тестовые функции бинарной оптимизации.
    Входные параметры:
     Type - тип тестовой функции.
    Возвращаемое значение:
     Точность вычислений.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_PrecisionOfCalculationsOfTestFunction_Real();

    return VMHL_Result;
}
//---------------------------------------------------------------------------
void MHL_SetToZeroCountOfFitness()
{
    /*
    Функция обнуляет количество вызовов целевой функции.
    Обязательно вызвать один раз перед вызовом алгоритмов оптимизации при исследовании эффективности
    алгоритмов оптимизации, где требуется контроль числа вызовов целевой функции.
    Входные параметры:
     Отсутствуют.
    Возвращаемое значение:
     Отсутствует.
    */
    CountOfFitness=0;
}
//---------------------------------------------------------------------------
double MHL_TestFunction_Binary(int *x, int VMHL_N)
{
    /*
    Общая тестовая функция для задач бинарной оптимизации. Включает в себя все тестовые функции.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     x - указатель на исходный массив;
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение тестовой функции в точке x.
    */
    double VMHL_Result = 0;

    if (VMHL_TypeOfTestFunction==TestFunction_SumVector)
        VMHL_Result = MHL_MaximumOrMinimumOfTestFunction_Binary()*MHL_TestFunction_SumVector(x, VMHL_N);
    CountOfFitness++;//увеличиваем число вызовов целевой функции
    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_TestFunction_Binary(int *x, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Общая тестовая функция для задач бинарной оптимизации. Включает в себя все тестовые функции.
    Входные параметры:
     x - указатель на исходный массив;
     VMHL_N - размер массива x,
     Type - тип тестовой функции.
     Смотреть виды в переменных перечисляемого типа в начале HarrixMathLibrary.h файла.
    Возвращаемое значение:
     Значение тестовой функции в точке x.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_TestFunction_Binary(x, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_TestFunction_Real(double *x, int VMHL_N)
{
    /*
    Общая тестовая функция для задач вещественной оптимизации. Включает в себя все тестовые функции.
    Обязательно вызвать один раз перед ее использованием функцию MHL_DefineTestFunction,
    в которой определяется конкретный тип задачи оптимизации.
    Входные параметры:
     x - указатель на исходный массив;
     VMHL_N - размер массива x.
    Возвращаемое значение:
     Значение тестовой функции в точке x.
    */
    double VMHL_Result = 0;

    if (VMHL_TypeOfTestFunction==TestFunction_Ackley)
        VMHL_Result = MHL_MaximumOrMinimumOfTestFunction_Real()*MHL_TestFunction_Ackley(x, VMHL_N);

    if (VMHL_TypeOfTestFunction==TestFunction_ParaboloidOfRevolution)
        VMHL_Result = MHL_MaximumOrMinimumOfTestFunction_Real()*MHL_TestFunction_ParaboloidOfRevolution(x, VMHL_N);

    if (VMHL_TypeOfTestFunction==TestFunction_Rastrigin)
        VMHL_Result = MHL_MaximumOrMinimumOfTestFunction_Real()*MHL_TestFunction_Rastrigin(x, VMHL_N);

    if (VMHL_TypeOfTestFunction==TestFunction_Rosenbrock)
        VMHL_Result = MHL_MaximumOrMinimumOfTestFunction_Real()*MHL_TestFunction_Rosenbrock(x, VMHL_N);

    CountOfFitness++;//увеличиваем число вызовов целевой функции
    return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_TestFunction_Real(double *x, int VMHL_N, TypeOfTestFunction Type)
{
    /*
    Общая тестовая функция для задач вещественной оптимизации. Включает в себя все тестовые функции.
    Входные параметры:
     x - указатель на исходный массив;
     VMHL_N - размер массива x,
     Type - тип тестовой функции.
     Смотреть виды в переменных перечисляемого типа в начале HarrixMathLibrary.h файла.
    Возвращаемое значение:
     Значение тестовой функции в точке x.
    */
    double VMHL_Result = 0;

    VMHL_TypeOfTestFunction = Type;

    VMHL_Result = MHL_TestFunction_Real(x, VMHL_N);

    return VMHL_Result;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Интегрирование
//*****************************************************************
double MHL_IntegralOfRectangle(double a, double b, double Epsilon,double (*Function)(double))
{
/*
Интегрирование по формуле прямоугольников с оценкой точности по правилу Рунге.
Считается интеграл функции на отрезке [a,b] с погрешностью порядка Epsilon.
Входные параметры:
 a - начало отрезка интегрирования;
 b - конец отрезка интегрирования;
 Epsilon - погрешность;
 Function - подынтегральная функция.
Возвращаемое значение:
 Значение определенного интеграла.
Примечание: значимые цифры в ответе определяются Epsilon.
*/
int i,n;
double h, s1, VMHL_Result;
n=1;
h=b-a;
VMHL_Result=h*Function((a+b)/2.);
do
 {
 n=2*n;
 s1=VMHL_Result;
 h=h/2.;
 VMHL_Result=0;
 i=1;
 do
  {
  VMHL_Result=VMHL_Result+Function(a+h/2.+h*(i-1.));//вычисляем интегральную сумму
  i=i+1;
  }
 while (i<=n);
 VMHL_Result=VMHL_Result*h;
 }
while (fabs(VMHL_Result-s1)>3.*Epsilon);
return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_IntegralOfSimpson(double a, double b, double Epsilon,double (*Function)(double))
{
/*
Интегрирование по формуле Симпсона с оценкой точности по правилу Рунге.
Считается интеграл функции на отрезке [a,b] с погрешностью порядка Epsilon.
Входные параметры:
 a - начало отрезка интегрирования;
 b - конец отрезка интегрирования;
 Epsilon - погрешность;
 Function - подынтегральная функция.
Возвращаемое значение:
 Значение определенного интеграла.
Примечание: значимые цифры в ответе определяются Epsilon.
*/
double h,s,s1,VMHL_Result,s3,x;
VMHL_Result=1.;
h=b-a;
s=Function(a)+Function(b);
do
 {
 s3=VMHL_Result;
 h=h/2.;
 s1=0;
 x=a+h;
 do
  {
  s1=s1+2.*Function(x);
  x=x+2.*h;
  }
 while (x<b);
 s=s+s1;
 VMHL_Result=(s+s1)*h/3.;
 x=fabs(s3-VMHL_Result)/15.;
}
while (x>Epsilon);
return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_IntegralOfTrapezium(double a, double b, double Epsilon,double (*Function)(double))
{
/*
Интегрирование по формуле трапеции с оценкой точности по правилу Рунге.
Считается интеграл функции на отрезке [a,b] с погрешностью порядка Epsilon.
Входные параметры:
 a - начало отрезка интегрирования;
 b - конец отрезка интегрирования;
 Epsilon - погрешность;
 Function - подынтегральная функция.
Возвращаемое значение:
 Значение определенного интеграла.
Примечание: значимые цифры в ответе определяются Epsilon.
*/
int i,n;
double h, s1, VMHL_Result;
n=1;
h=b-a;
VMHL_Result=h*(Function(a)+Function(b))/2.;
do
 {
 s1=VMHL_Result;
 VMHL_Result=0;
 i=1;
 do
  {
  VMHL_Result=VMHL_Result+Function(a-h/2.+h*i);//вычисляем интегральную сумму
  i=i+1;
  }
 while (i<=n);
 VMHL_Result=s1/2.+VMHL_Result*h/2.;
 n=2*n;
 h=h/2.;
 }
while (fabs(VMHL_Result-s1)>3.*Epsilon);
return VMHL_Result;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Кодирование и декодирование
//*****************************************************************
void MHL_BinaryGrayVectorToRealVector(int *x, int n, double *VMHL_ResultVector, double *Left, double *Right, int *Lengthi, int VMHL_N)
{
/*
Функция декодирует бинарную строку в действительный вектор, который и был закодирован
методом "Стандартный рефлексивный Грей-код" (без использования временного массива).
Входные параметры:
 a - бинарная строка представляющая собой Грей-код нескольких закодированных вещественных координат;
 n - длина бинарной строки;
 VMHL_ResultVector - вещественный вектор, в который мы и записываем результат, размера n;
 Left - массив левых границ изменения каждой вещественной координаты (размер VMHL_N);
 Right - массив правых границ изменения каждой вещественной координаты (размер VMHL_N);
 Lengthi - массив значений, сколько на каждую координату отводится бит в бинарной строке (размер массива Lengthi VMHL_N);
 VMHL_N - длина вещественного вектора.
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
for (int i=0;i<VMHL_N;i++)
 {
 len=Lengthi[i];
 TMHL_GrayCodeToBinaryFromPart(x,TempBinaryVector,Begin,len);
 Begin+=len;
 }

//Переводим наш полученный бинарный код в вещественный вектор
MHL_BinaryVectorToRealVector(TempBinaryVector,VMHL_ResultVector,Left,Right,Lengthi,VMHL_N);

delete [] TempBinaryVector;
}
//---------------------------------------------------------------------------
void MHL_BinaryGrayVectorToRealVector(int *x, double *VMHL_ResultVector,int *TempBinaryVector, double *Left, double *Right, int *Lengthi, int VMHL_N)
{
/*
Функция декодирует бинарную строку в действительный вектор, который и был закодирован
методом "Стандартный рефлексивный Грей-код".
Входные параметры:
 a - бинарная строка представляющая собой Грей-код нескольких закодированных вещественных координат;
 VMHL_ResultVector - вещественный вектор, в который мы и записываем результат;
 TempBinaryVector - указатель на временный массив  размера n;
 Left - массив левых границ изменения каждой вещественной координаты размера VMHL_N;
 Right - массив правых границ изменения каждой вещественной координаты размера VMHL_N;
 Lengthi - массив значений, сколько на каждую координату отводится бит в бинарной строке. Размер массива VMHL_N;
 VMHL_N - длина вещественного вектора.
Возвращаемое значение:
 Отсутствует.
Примечание:
 К криптографии данная функция не имеет отношения.
*/
int len;//Сколько на текущую координату отводится бит в бинарной строке
int Begin=0;//Номер бита в бинарной строке, с которой начинается текущая закодированная вещественная координата

//Вначале нужно перевести бинарную строку кода Грея в бинарную строку
for (int i=0;i<VMHL_N;i++)
 {
 len=Lengthi[i];
 TMHL_GrayCodeToBinaryFromPart(x,TempBinaryVector,Begin,len);
 Begin+=len;
 }

//Переводим наш полученный бинарный код в вещественный вектор
MHL_BinaryVectorToRealVector(TempBinaryVector,VMHL_ResultVector,Left,Right,Lengthi,VMHL_N);
}
//---------------------------------------------------------------------------
void MHL_BinaryVectorToRealVector(int *x, double *VMHL_ResultVector, double *Left, double *Right, int *Lengthi, int VMHL_N)
{
/*
Функция декодирует бинарную строку в действительный вектор, который и был закодирован
методом "Стандартное представление целого числа – номер узла в сетке дискретизации".
Входные параметры:
 a - бинарная строка;
 VMHL_ResultVector - вещественный вектор, в который мы и записываем результат;
 Left - массив левых границ изменения каждой вещественной координаты;
 Right - массив правых границ изменения каждой вещественной координаты;
 Lengthi - массив значений, сколько на каждую координату отводится бит в бинарной строке;
 VMHL_N - длина вещественного вектора.
Возвращаемое значение:
 Отсутствует.
Примечание:
 К криптографии данная функция не имеет отношения.
Примечание:
 Вектор входный параметров действительно избыточен, но каждый раз пересчитывать затратно, так как функция вызывается в ГА часто.
*/
int len;//Сколько на текущую координату отводится бит в бинарной строке
double l;//Левая граница текущей координаты вещественного вектора
double r;//Правая граница текущей координаты вещественного вектора
double count;//Сколько может быть закодировано целых чисел двоичным числом длины len
int Begin=0;//Номер бита в бинарной строке, с которой начинается текущая закодированная вещественная координата

for (int i=0;i<VMHL_N;i++)
 {
 len=Lengthi[i];
 count=double(TMHL_PowerOf(2,len));
 l=Left[i];
 r=Right[i];
 VMHL_ResultVector[i]=l+(r-l)*double(TMHL_BinaryToDecimalFromPart(x,Begin,len))/count;
 Begin+=len;
 }
}
//---------------------------------------------------------------------------

//*****************************************************************
//Комбинаторика
//*****************************************************************

//*****************************************************************
//Математические функции
//*****************************************************************
double MHL_ArithmeticalProgression(double a1,double d,int n)
{
/*
Арифметическая прогрессия. n-ый член последовательности.
Входные параметры:
 a1 - начальный член прогрессии;
 d - шаг арифметической прогрессии;
 n - номер последнего члена.
Возвращаемое значение:
 n-ый член последовательности.
*/
double VMHL_Result=a1+d*(n-1.);
return  VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_ExpMSxD2(double x)
{
/*
Функция вычисляет выражение exp(-x*x/2).
Входные параметры:
 x - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
return exp(-x*x/2.);
}
//---------------------------------------------------------------------------
double MHL_GeometricSeries(double u1,double q,int n)
{
/*
Геометрическая прогрессия. n-ый член последовательности.
Входные параметры:
 u1 - начальный член прогрессии;
 q - шаг  прогрессии;
 n - номер последнего члена.
Возвращаемое значение:
 n-ый член последовательности.
*/
double qn1;
qn1=TMHL_PowerOf(q,n-1);
double VMHL_Result=u1*qn1;
return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_GreatestCommonDivisorEuclid(int A,int B)
{
/*
Функция находит наибольший общий делитель двух чисел по алгоритму Евклида.
Входные параметры:
 A - первое число;
 B - второе число.
Возвращаемое значение:
 НОД(A,B)
*/
int a,b;
//A должен быть не меньше B
if (A>=B)
 {
 a=abs(A);
 b=abs(B);
 }
else
 {
 a=abs(B);
 b=abs(A);
 }
while((a!=0)&&(b!=0))
 {
 if (a>=b)
  a=a%b;
 else
  b=b%a;
 }
int VMHL_Result=a+b;
return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_HowManyPowersOfTwo(int x)
{
/*
Функция вычисляет, какой минимальной степенью двойки можно покрыть целое положительное число.
Входные параметры:
 x - целое число.
Возвращаемое значение:
 Минимальная степень двойки можно покрыть целое положительное число: min(2^VMHL_Result)>x
*/
x=abs(x);
double m=1;
int VMHL_Result=0;
while (m<x)
 {
 m*=2;
 VMHL_Result++;
 }
return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_InverseNormalizationNumberAll(double x)
{
/*
Функция осуществляет обратную нормировку числа из интервала [0;1]  в интервал [-бесконечность;+бесконечность],
которое было осуществлено функцией MHL_NormalizationNumberAll.
Под бесконечностью принимается машинная бесконечность.
Входные параметры:
 x - число в интервале [0;1].
Возвращаемое значение:
 Перенормированное число.
*/
if (x==0.5)
 return 0.;
if (x>=1)
 return MHL_INFINITY;
if (x<=0)
 return MHL_MINFINITY;
if (x>0.5)
 return (1./((1./(2.*x-1.))-1.));
else
 return (1./(1.-(1./(1.-2.*x))));
}
//---------------------------------------------------------------------------
int MHL_LeastCommonMultipleEuclid(int A,int B)
{
/*
Функция находит наименьшее общее кратное двух чисел по алгоритму Евклида.
Входные параметры:
 A - первое число;
 B - второе число.
Возвращаемое значение:
 НОК(A,B)
*/
A=abs(A);
B=abs(B);
int gcd,VMHL_Result;
//НОК вычисляем на основе НОД
gcd=MHL_GreatestCommonDivisorEuclid(A,B);
VMHL_Result=(A*B)/gcd;
return VMHL_Result;
}
//---------------------------------------------------------------------------
void MHL_MixedMultiLogicVectorOfFullSearch(int *VMHL_Vector, int I, int *HowMuchInElements, int VMHL_N)
{
/*
Функция генерирует определенный вектор k-значной логики, где каждый элемент может
принимать разное максимальное значение, в полном переборе вариантов.
Генерируется I вектор в этом полном переборе.
Входные параметры:
 VMHL_Vector - выходной вектор, в который записывется результат;
 I - номер в массиве в полном переборе, начиная с нуля (от 0 и до произведения всех элементов массива HowMuchInElements - 1);
 HowMuchInElements - сколько значений может принимать элемент в векторе. То есть элемент может быть 0 и HowMuchInElements[i] - 1;
 VMHL_N - количество элементов в массиве.
Возвращаемое значение:
 Отсутствует.
*/
    int *CountInBlock = new int[VMHL_N];

    int CountOfAllVariants = TMHL_ProductOfElementsOfVector(HowMuchInElements, VMHL_N);

    CountInBlock[0] = CountOfAllVariants/HowMuchInElements[0];

    for (int i=1;i<VMHL_N;i++)
        CountInBlock[i] = CountInBlock[i-1]/HowMuchInElements[i];

    for (int i=0;i<VMHL_N;i++)
        VMHL_Vector[i] = (I/CountInBlock[i])%HowMuchInElements[i];

    delete [] CountInBlock;
}
//---------------------------------------------------------------------------
double MHL_NormalizationNumberAll(double x)
{
/*
Функция нормирует число из интервала [бесконечность;+бесконечность] в интервал [0;1].
При этом в нуле возвращает 0.5, в -бесконечность возвращает 0, в +бесконечность
возвращает 1. Если x<y, то MHL_NormalizationNumberAll(x)<MHL_NormalizationNumberAll(y).
Под бесконечностью принимается машинная бесконечность.
Входные параметры:
 x - число.
Возвращаемое значение:
 Нормированное число.
*/
if (x==0) return 0.5;
if (x>=MHL_INFINITY) return 1.;
if (x<=MHL_MINFINITY) return 0.;
return ((1./(1.+1./fabs(x))*TMHL_Sign(x)+1.)/2.);
}
//---------------------------------------------------------------------------
int MHL_Parity(int a)
{
/*
Функция проверяет четность целого числа.
Входные параметры:
 a - исходное число.
Возвращаемое значение:
 1 - четное;
 0 - нечетное.
*/
if (a%2==0)
 return 1;
else
 return 0;
}
//---------------------------------------------------------------------------
double MHL_SumGeometricSeries(double u1,double q,int n)
{
/*
Геометрическая прогрессия. Сумма первых n членов.
Входные параметры:
 u1 - начальный член прогрессии;
 q - шаг  прогрессии;
 n - номер последнего члена.
Возвращаемое значение:
 Сумма первых n членов.
*/
double qn;
qn=TMHL_PowerOf(q,n);
double VMHL_Result=u1*(1.-qn)/(1.-q);
return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_SumOfArithmeticalProgression(double a1,double d,int n)
{
/*
Арифметическая прогрессия. Сумма первых n членов.
Входные параметры:
 a1 - начальный член прогрессии;
 d - шаг арифметической прогрессии;
 n - номер последнего члена.
Возвращаемое значение:
 Сумма первых n членов.
*/
double VMHL_Result=(2.*a1+d*(n-1.))*n/2.;
return  VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_SumOfDigits(int a)
{
/*
Функция подсчитывает сумму цифр любого целого числа.
Входные параметры:
 a - целое число.
Возвращаемое значение:
 Cумма цифр.
*/
if (a<1) a=-a;
int VMHL_Result=0;
while (a>=1)
 {
 VMHL_Result+=a-int(int(a/10)*10.);
 a/=10;
 }
return VMHL_Result;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Матрицы
//*****************************************************************

//*****************************************************************
//Метрика
//*****************************************************************

//*****************************************************************
//Непараметрика
//*****************************************************************
double MHL_BellShapedKernelExp(double z)
{
/*
Колоколообразное экспоненциальное ядро.
Входные параметры:
 z - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
double f=0;
z=fabs(z);
if (z<=2.47638181818)
    f=0.05968*(exp(z)+exp(-z))-0.154293*z*z+0.2311;
return f;
}
//---------------------------------------------------------------------------
double MHL_BellShapedKernelParabola(double z)
{
/*
Колоколообразное параболическое ядро.
Входные параметры:
z - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
double f=0;
if (z*z<=5)
    f=0.335-0.067*z*z;
return f;
}
//---------------------------------------------------------------------------
double MHL_BellShapedKernelRectangle(double z)
{
/*
Колоколообразное прямоугольное ядро.
Входные параметры:
z - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
double f=0;
z=fabs(z);
if (z<=1)
    f=0.5;
return f;
}
//---------------------------------------------------------------------------
double MHL_BellShapedKernelTriangle(double z)
{
/*
Колоколообразное треугольное ядро.
Входные параметры:
z - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
double f=0;
z=fabs(z);
if (z<=1)
    f=1.-z;
return f;
}
//---------------------------------------------------------------------------
double MHL_DerivativeOfBellShapedKernelExp(double z)
{
/*
Производная колоколообразного экспоненциального ядра.
Входные параметры:
 z - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
double f=0;
if (fabs(z)<=2.47638181818) f=0.05968*(exp(z)-exp(-z))-0.308586*z;
return f;
}
//---------------------------------------------------------------------------
double MHL_DerivativeOfBellShapedKernelParabola(double z)
{
/*
Производная колоколообразного параболического ядра.
Входные параметры:
 z - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
double f=0;
if (z*z<=5) f=-0.134*z;
return f;
}
//---------------------------------------------------------------------------
double MHL_DerivativeOfBellShapedKernelRectangle(double z)
{
/*
Производная колоколообразного прямоугольного ядра.
Входные параметры:
z - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
double f=0;
if (z==1) f=-1.7E308;
if (z==-1) f=1.7E308;
return f;
}
//---------------------------------------------------------------------------
double MHL_DerivativeOfBellShapedKernelTriangle(double z)
{
/*
Производная колоколообразного треугольного ядра.
Входные параметры:
z - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
double f=0;
if ((z<=1)&&(z>=0)) f=-1.;
if ((z<0)&&(z>=-1)) f=1.;
return f;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Нечеткие системы
//*****************************************************************
double MHL_TrapeziformFuzzyNumber(double x,double a,double b,double c,double d)
{
/*
Трапециевидное нечеткое число. Точнее его функция принадлежности.
Входные параметры:
 x - действительное число, для которого считаем функцию принадлежности.
 a - левая крайняя граница;
 b - начало устойчивой области;
 с - конец устойчивой области;
 d - правая крайняя граница.
Возвращаемое значение:
 Значение функции принадлежности.
*/
double f=0;

if (x<a) f=0.;
if ((x>=a)&&(x<b)) f=(x-a)/(b-a);
if ((x>=b)&&(x<=c)) f=1.;
if ((x>c)&&(x<=d)) f=(d-x)/(d-c);
if (x>d) f=0.;

return f;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Оптимизация
//*****************************************************************
int MHL_BinaryMonteCarloAlgorithm(int *Parameters, double (*FitnessFunction)(int*,int), int *VMHL_ResultVector, double *VMHL_Result)
{
/*
Метод Монте-Карло (Monte-Carlo). Простейший метод оптимизации на бинарных строках. В простонародье его называют "методом научного тыка".
Алгоритм оптимизации. Ищет максимум функции пригодности FitnessFunction.
Входные параметры:
 Parameters
  [0] - длина бинарной строки (определяется задачей оптимизации, что мы решаем);
  [1] - число вычислений функции пригодности (CountOfFitness);
 FitnessFunction - указатель на целевую функцию (если решается задача условной оптимизации, то учет ограничений должен быть включен в эту функцию);
 VMHL_ResultVector - найденное решение (бинарный вектор);
 VMHL_Result - значение функции в точке, определенной вектором VMHL_ResultVector.
Возвращаемое значение:
 1 - завершил работу без ошибок. Всё хорошо.
 0 - возникли при работе ошибки. Скорее всего в этом случае в VMHL_ResultVector и в VMHL_Result не содержится решение задачи.
Пример значений рабочего вектора Parameters:
 Parameters[0]=20;
 Parameters[1]=100*100;
*/
//Считываем из Parameters параметры алгоритма
int LengthBinarString=Parameters[0];//Длина бинарной строки
int CountOfFitness=Parameters[1];//Число вычислений функции целевой

//Проверим данные
if (LengthBinarString<1) return 0;//Слишком маленькая длина бинарной строки
if (CountOfFitness<1) return 0;//Слишком маленькое число вычислений функции целевой

//Переменные
int i;//Счетчик
double TempFitness;//Значение функции целевой произвольного решения
double BestFitness;//Значение функции целевой лучшего решения за всё время работы алгоритма

//Для выполнения алгоритма требуются некоторые дополнительные массивы. Создадим их.
//Массив для хранения произвольного решения
int *TempIndividual;
TempIndividual=new int[LengthBinarString];
//Массив для хранения лучшего решения за всё время работы алгоритма
int *BestIndividual;
BestIndividual=new int[LengthBinarString];

//Получим первое решение (оно пока и лучшее)
TMHL_RandomBinaryVector(BestIndividual,LengthBinarString);

//Вычислим значение функции целевой
try
 {
 BestFitness=FitnessFunction(BestIndividual,LengthBinarString);
 }
catch(...)
 {
 return 0;//Алгоритм не смог посчитать значение функции целевой
 }

for (i=1;i<CountOfFitness;i++)
 {//////////////////// ГЛАВНЫЙ ЦИКЛ ///////////////////////

 //Получим новое случайное решение
 TMHL_RandomBinaryVector(TempIndividual,LengthBinarString);

 //Вычислим значение функции целевой
 try
  {
  TempFitness=FitnessFunction(TempIndividual,LengthBinarString);
  }
 catch(...)
  {
  return 0;//Алгоритм не смог посчитать значение функции целевой
  }

 //Является ли данное решение лучше лучшего решения за всё время работы алгоритма
 if (TempFitness>BestFitness)
  {
  //Если всё-таки лучше
  TMHL_VectorToVector(TempIndividual,BestIndividual,LengthBinarString);
  BestFitness=TempFitness;//Запоминаем его значение функции целевой
  }

 }//////////////////// ГЛАВНЫЙ ЦИКЛ ///////////////////////

//Алгоритм закончил свою работу
//Выдадим найденное лучшее решение за время запуска алгоритма и его значение функции целевой
TMHL_VectorToVector(BestIndividual,VMHL_ResultVector,LengthBinarString);
*VMHL_Result=BestFitness;

//Удалим все дополнительные массивы
delete [] TempIndividual;
delete [] BestIndividual;

return 1;//Всё успешно
}
//---------------------------------------------------------------------------
void MHL_DichotomyOptimization (double Left, double Right, double (*Function)(double), double Interval, double Epsilon, double *VMHL_Result_X,double *VMHL_Result_Y)
{
/*
Метод дихотомии. Метод одномерной оптимизации унимодальной функции на интервале. Ищет минимум.
Входные параметры:
 Left - начало интервала поиска;
 Right - конец интервала поиска;
 Function - унимодальная функция, минимум которой ищется;
 Interval - длина конечного интервала неопределенности (точность поиска);
 Epsilon - малое число;
 VMHL_Result_X - вычисленная точка минимума (сюда записывается результат);
 VMHL_Result_Y - значение функции в точке минимума (сюда записывается результат).
Возвращаемое значение:
 Отсутствует.
Примечание:
 Epsilon должен быть меньше, чем Interval/2, иначе возвращается левая граница просто.
*/
if (Epsilon>=Interval/2.)
 {
 //зануляем результаты
 *VMHL_Result_X=Left;
 *VMHL_Result_Y=Function(*VMHL_Result_X);
 }
else
 {
 double l,v;
 while (fabs(Right-Left)>=Interval)
  {
  l=(Left+Right)/2.-Epsilon;
  v=(Left+Right)/2.+Epsilon;
  if (Function(l)<Function(v))
   Right=v;
  else
   Left=l;
  }
 *VMHL_Result_X=(Left+Right)/2.;
 *VMHL_Result_Y=Function(*VMHL_Result_X);
 }
}
//---------------------------------------------------------------------------
void MHL_FibonacciOptimization (double Left, double Right, double (*Function)(double), int Count, double *VMHL_Result_X,double *VMHL_Result_Y)
{
/*
Метод Фибоначчи. Метод одномерной оптимизации унимодальной функции на интервале. Ищет минимум.
Входные параметры:
 Left - начало интервала поиска;
 Right - конец интервала поиска;
 Function - унимодальная функция, минимум которой ищется;
 Count - число вычислений целевой функции;
 VMHL_Result_X - вычисленная точка минимума (сюда записывается результат);
 VMHL_Result_Y - значение функции в точке минимума (сюда записывается результат).
Возвращаемое значение: 
 Отсутствует.
*/
double l,v,fl,fv,Fib1,Fib2;
int i=1,n;
n=Count-1;

Fib1=TMHL_FibonacciNumber(n-i-1);
Fib2=TMHL_FibonacciNumber(n-i+1);
l=Left+(Fib1/Fib2)*(Right-Left);

Fib1=TMHL_FibonacciNumber(n-i  );
Fib2=TMHL_FibonacciNumber(n-i+1);
v=Left+(Fib1/Fib2)*(Right-Left);

fl=Function(l);
fv=Function(v);
for (i=2;i<n;i++)
 {
 if (Function(l)<Function(v))
  {
  Right=v;
  v=l;
  fv=fl;

  Fib1=TMHL_FibonacciNumber(n-i-1);
  Fib2=TMHL_FibonacciNumber(n-i+1);
  l=Left+(Fib1/Fib2)*(Right-Left);

  fl=Function(l);
  }
 else
  {
  Left=l;
  l=v;
  fl=fv;

  Fib1=TMHL_FibonacciNumber(n-i  );
  Fib2=TMHL_FibonacciNumber(n-i+1);
  v=Left+(Fib1/Fib2)*(Right-Left);

  fv=Function(v);
  }
 }
*VMHL_Result_X=(Left+Right)/2.;
*VMHL_Result_Y=Function(*VMHL_Result_X);
}
//---------------------------------------------------------------------------
void MHL_GoldenSectionOptimization (double Left, double Right, double (*Function)(double), double Interval, double *VMHL_Result_X,double *VMHL_Result_Y)
{
/*
Метод золотого сечения. Метод одномерной оптимизации унимодальной функции на интервале. Ищет минимум.
Входные параметры:
 Left - начало интервала поиска;
 b - конец интервала поиска;
 Function - унимодальная функция, минимум которой ищется;
 Interval - длина конечного интервала неопределенности (точность поиска);
 VMHL_Result_X - вычисленная точка минимума (сюда записывается результат);
 VMHL_Result_Y - значение функции в точке минимума (сюда записывается результат).
Возвращаемое значение: 
 Отсутствует.
*/
double l,v,fl,fv;
l=Left+(1.-MHL_TAU)*(Right-Left);
v=Left+MHL_TAU*(Right-Left);
fl=Function(l);
fv=Function(v);
while (fabs(Right-Left)>=Interval)
 {
 if (Function(l)<Function(v))
  {
  Right=v;
  v=l;
  fv=fl;
  l=Left+(1.-MHL_TAU)*(Right-Left);
  fl=Function(l);
  }
 else
  {
  Left=l;
  l=v;
  fl=fv;
  v=Left+MHL_TAU*(Right-Left);
  fv=Function(v);
  }
 }
*VMHL_Result_X=(Left+Right)/2.;
*VMHL_Result_Y=Function(*VMHL_Result_X);
}
//---------------------------------------------------------------------------
void MHL_QuadraticFitOptimization (double Left, double Right, double (*Function)(double), double Epsilon, double Epsilon2, double *VMHL_Result_X,double *VMHL_Result_Y)
{
/*
Метод квадратичной интерполяции. Метод одномерной оптимизации унимодальной функции на интервале. Ищет минимум.
Входные параметры:
 Left - начало интервала поиска;
 Right - конец интервала поиска;
 Function - унимодальная функция, минимум которой ищется;
 Epsilon - точность;
 Epsilon2 - шаг, фактически еще одно малое число;
 VMHL_Result_X - вычисленная точка минимума (сюда записывается результат);
 VMHL_Result_Y - значение функции в точке минимума (сюда записывается результат).
Возвращаемое значение:
 Отсутствует.
*/
double x,h,x1,x2,x3,k,f1,f2,f3,fx,fp1,fp2,fp3,p[4],X=Left;
double error=0;
int ko;
h=Epsilon2;
x1=(Left+Right)/2.-h;
x2=(Left+Right)/2.;
x3=(Left+Right)/2.+h;
f1=Function(x1);
f2=Function(x2);
f3=Function(x3);
if (f1<f2)
 k=-1.;
else
 k=1.;
while (!((f1>=f2)&&(f2<=f3)))
 {
 x1=x1+k*h;
 x2=x2+k*h;
 x3=x3+k*h;
 f1=Function(x1);
 f2=Function(x2);
 f3=Function(x3);
 if (x1<Left) error=Left;
 if (x3>Right) error=Right;
 if (error!=0) break;
 }
if (error==0)
{
 x=0.5*((x2*x2-x3*x3)*f1+(x3*x3-x1*x1)*f2+(x1*x1-x2*x2)*f3)/((x2-x3)*f1+(x3-x1)*f2+(x1-x2)*f3);
 fx=Function(x);
 while ((fabs(fx-f2)>Epsilon)&&(X!=x))
  {
  X=x;
  p[0]=x1;
  p[1]=x2;
  p[2]=x3;
  p[3]=x;
  TMHL_BubbleSort(p, 4);
  fp1=Function(p[0]);
  fp2=Function(p[1]);
  fp3=Function(p[2]);
  if (!((fp1>=fp2)&&(fp2<=fp3)))
   ko=1;
  else
   ko=0;
  x1=p[0+ko];
  x2=p[1+ko];
  x3=p[2+ko];
  x=0.5*((x2*x2-x3*x3)*f1+(x3*x3-x1*x1)*f2+(x1*x1-x2*x2)*f3)/((x2-x3)*f1+(x3-x1)*f2+(x1-x2)*f3);
  fx=Function(x);
  }
 *VMHL_Result_X=x;
 *VMHL_Result_Y=fx;
}
else
 {
    *VMHL_Result_X=error;
    *VMHL_Result_Y=Function(error);
 }
}
//---------------------------------------------------------------------------
int MHL_RealMonteCarloAlgorithm(int *Parameters, double *Left, double *Right, double (*FitnessFunction)(double*,int), double *VMHL_ResultVector, double *VMHL_Result)
{
/*
Метод Монте-Карло (Monte-Carlo). Простейший метод оптимизации на вещественных строках. В простонародье его называют "методом научного тыка".
Алгоритм оптимизации. Ищет максимум функции пригодности FitnessFunction.
Входные параметры:
 Parameters
  [0] - длина вещественной строки (определяется задачей оптимизации, что мы решаем);
  [1] - число вычислений функции пригодности (CountOfFitness);
 Left - массив левых границ изменения каждой вещественной координаты (размерность Parameters[0]);
 Right - массив правых границ изменения каждой вещественной координаты (размерность Parameters[0]);
 FitnessFunction - указатель на целевую функцию (если решается задача условной оптимизации, то учет ограничений должен быть включен в эту функцию);
 VMHL_ResultVector - найденное решение (вещественный вектор);
 VMHL_Result - значение функции в точке, определенной вектором VMHL_ResultVector.
Возвращаемое значение:
 1 - завершил работу без ошибок. Всё хорошо.
 0 - возникли при работе ошибки. Скорее всего в этом случае в VMHL_ResultVector и в VMHL_Result не содержится решение задачи.
Пример значений рабочего вектора Parameters:
 Parameters[0]=5;
 Parameters[1]=50*50;
*/
//Считываем из Parameters параметры алгоритма
int LengthBinarString=Parameters[0];//Длина вещественной строки
int CountOfFitness=Parameters[1];//Число вычислений функции целевой

//Проверим данные
if (LengthBinarString<1) return 0;//Слишком маленькая длина вещественной строки
if (CountOfFitness<1) return 0;//Слишком маленькое число вычислений функции целевой

//Переменные
int i;//Счетчик
double TempFitness;//Значение функции пригодности произвольного решения
double BestFitness;//Значение функции пригодности лучшего решения за всё время работы алгоритма

//Для выполнения алгоритма требуются некоторые дополнительные массивы. Создадим их.
//Массив для хранения произвольного решения
double *TempIndividual;
TempIndividual=new double[LengthBinarString];
//Массив для хранения лучшего решения за всё время работы алгоритма
double *BestIndividual;
BestIndividual=new double[LengthBinarString];

//Получим первое решение (оно пока и лучшее)
MHL_RandomRealVectorInElements(BestIndividual,Left,Right,LengthBinarString);

//Вычислим значение функции целевой
try
 {
 BestFitness=FitnessFunction(BestIndividual,LengthBinarString);
 }
catch(...)
 {
 return 0;//Алгоритм не смог посчитать значение функции целевой
 }

for (i=1;i<CountOfFitness;i++)
 {//////////////////// ГЛАВНЫЙ ЦИКЛ ///////////////////////

 //Получим новое случайное решение
 MHL_RandomRealVectorInElements(TempIndividual,Left,Right,LengthBinarString);

 //Вычислим значение функции целевой
 try
  {
  TempFitness=FitnessFunction(TempIndividual,LengthBinarString);
  }
 catch(...)
  {
  return 0;//Алгоритм не смог посчитать значение функции целевой
  }

 //Является ли данное решение лучше лучшего решения за всё время работы алгоритма
 if (TempFitness>BestFitness)
  {
  //Если всё-таки лучше
  TMHL_VectorToVector(TempIndividual,BestIndividual,LengthBinarString);
  BestFitness=TempFitness;//Запоминаем его значение функции целевой
  }

 }//////////////////// ГЛАВНЫЙ ЦИКЛ ///////////////////////

//Алгоритм закончил свою работу
//Выдадим найденное лучшее решение за время запуска алгоритма и его значение функции целевой
TMHL_VectorToVector(BestIndividual,VMHL_ResultVector,LengthBinarString);
*VMHL_Result=BestFitness;

//Удалим все дополнительные массивы
delete [] TempIndividual;
delete [] BestIndividual;

return 1;//Всё успешно
}
//---------------------------------------------------------------------------
void MHL_RealMonteCarloOptimization (double Left, double Right, double (*Function)(double), int Count, double *VMHL_Result_X,double *VMHL_Result_Y)
{
/*
Метод Монте-Карло (Monte-Carlo). Простейший метод оптимизации на вещественных строках. Ищет минимум.
От функции MHL_RealMonteCarloAlgorithm отличается тем, что ищет минимум, а не максимум, и не у многомерной функции,
а одномерной. Вводится, чтобы было продолжением однотипных методов оптимизации одномерных унимодальных функций.
Входные параметры:
 Left - начало интервала поиска
 Right - конец интервала поиска
 Function - унимодальная функция, минимум которой ищется
 Count - число вычислений целевой функции
 VMHL_Result_X - вычисленная точка минимума (сюда записывается результат);
 VMHL_Result_Y - значение функции в точке минимума (сюда записывается результат).
Возвращаемое значение:
 Отсутствует.
*/
int i;
double min,fmin,f,x;
min=MHL_RandomUniform(Left,Right);
fmin=Function(min);
for (i=1;i<Count;i++)
 {
 x=MHL_RandomUniform(Left,Right);
 f=Function(x);
 if (f<fmin)
  {
  min=x;
  fmin=f;
  }
 }
*VMHL_Result_X=min;
*VMHL_Result_Y=fmin;
}
//---------------------------------------------------------------------------
void MHL_UniformSearchOptimization (double Left, double Right, double (*Function)(double), double Interval, double *VMHL_Result_X,double *VMHL_Result_Y)
{
/*
Метод равномерного поиска. Метод одномерной оптимизации функции на интервале. Ищет минимум.
Входные параметры:
 Left - начало интервала поиска;
 Right - конец интервала поиска;
 Function - унимодальная функция, минимум которой ищется;
 Interval - длина шага, с которым будет проводится поиск;
 VMHL_Result_X - вычисленная точка минимума (сюда записывается результат);
 VMHL_Result_Y - значение функции в точке минимума (сюда записывается результат).
Возвращаемое значение:
 Отсутствует.
*/
int i,n;
double min,fmin,f,x;
n=ceil(fabs(Right-Left)/Interval);
min=Left;
fmin=Function(min);
x=Left;
for (i=1;i<n;i++)
 {
 x+=Interval;
 f=Function(x);
 if (f<fmin)
  {
  min=x;
  fmin=f;
  }
 }
*VMHL_Result_X=min;
*VMHL_Result_Y=fmin;
}
//---------------------------------------------------------------------------
void MHL_UniformSearchOptimizationN (double Left, double Right, double (*Function)(double), int Count, double *VMHL_Result_X,double *VMHL_Result_Y)
{
/*
Метод равномерного поиска. Метод одномерной оптимизации функции на интервале. Ищет минимум. От MHL_UniformSearchOptimization отличается тем, что вместо параметра шага равномерного прохода используется число вычислений целевой функции, но они взаимозаменяемы.
Входные параметры:
 Left - начало интервала поиска;
 Right - конец интервала поиска;
 Function - унимодальная функция, минимум которой ищется;
 Count - число вычислений целевой функции;
 VMHL_Result_X - вычисленная точка минимума (сюда записывается результат);
 VMHL_Result_Y - значение функции в точке минимума (сюда записывается результат).
Возвращаемое значение:
 Отсутствует.
*/
int i,n;
double min,fmin,f,x;
double Interval=ceil(fabs(Right-Left)/double(Count));
n=Count;
min=Left;
fmin=Function(min);
x=Left;
for (i=1;i<n;i++)
 {
 x+=Interval;
 f=Function(x);
 if (f<fmin)
  {
  min=x;
  fmin=f;
  }
 }
*VMHL_Result_X=min;
*VMHL_Result_Y=fmin;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Перевод единиц измерений
//*****************************************************************
double MHL_DegToRad(double VMHL_X)
{
/*
Функция переводит угол из градусной меры в радианную.
Входные параметры:
 VMHL_X - градусная мера угла.
Возвращаемое значение:
 Радианная мера угла.
*/
return (MHL_PI*VMHL_X)/180.;
}
//---------------------------------------------------------------------------
double MHL_RadToDeg(double VMHL_X)
{
/*
Функция переводит угол из радианной меры в градусную.
Входные параметры:
 VMHL_X - радианная мера угла.
Возвращаемое значение:
 Градусная мера угла.
*/
return (180.*VMHL_X)/MHL_PI;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Случайные объекты
//*****************************************************************
int MHL_BitNumber(double P)
{
/*
Функция с вероятностью P возвращает 1. В противном случае возвращает 0.
Входные параметры:
 P - вероятность появления 1.
Возвращаемое значение:
 1 или 0.
*/
if (MHL_RandomNumber()<=P)
 return 1;
else
 return 0;
}
//---------------------------------------------------------------------------
int MHL_BitNumber()
{
/*
Функция с вероятностью 0.5 возвращает 1. В противном случае возвращает 0.
Входные параметры:
 Отсутствуют.
Возвращаемое значение:
 1 или 0.
*/
if (MHL_RandomNumber()<=0.5)
 return 1;
else
 return 0;
}
//---------------------------------------------------------------------------
void MHL_RandomRealMatrix(double **VMHL_ResultMatrix, double Left, double Right, int VMHL_N, int VMHL_M)
{
/*
Функция заполняет матрицу случайными вещественными числами из определенного интервала [Left;Right].
Входные параметры:
 VMHL_ResultMatrix - указатель на матрицу;
 Left - левая граница интервала;
 Right - правая граница интервала;
 VMHL_N - размер массива (число строк);
 VMHL_M - размер массива (число столбцов).
Возвращаемое значение:
 Отсутствует.
*/
for (int i=0;i<VMHL_N;i++)
    for (int j=0;j<VMHL_M;j++)
        VMHL_ResultMatrix[i][j]=MHL_RandomUniform(Left,Right);
}
//---------------------------------------------------------------------------
void MHL_RandomRealMatrixInCols(double **VMHL_ResultMatrix, double *Left, double *Right, int VMHL_N, int VMHL_M)
{
/*
Функция заполняет матрицу случайными вещественными числами из определенного интервала.
При этом элементы каждого столбца изменяются в своих пределах.
Входные параметры:
 VMHL_ResultMatrix - указатель на матрицу;
 Left - левые границы интервала изменения элементов столбцов (размер VMHL_M);
 Right - правые границы интервала изменения элементов столбцов (размер VMHL_M);
 VMHL_N - размер массива (число строк);
 VMHL_M - размер массива (число столбцов).
Возвращаемое значение:
 Отсутствует.
*/
for (int i=0;i<VMHL_N;i++)
 for (int j=0;j<VMHL_M;j++)
  VMHL_ResultMatrix[i][j]=MHL_RandomUniform(Left[j],Right[j]);
}
//---------------------------------------------------------------------------
void MHL_RandomRealMatrixInElements(double **VMHL_ResultMatrix, double **Left, double **Right, int VMHL_N, int VMHL_M)
{
/*
Функция заполняет матрицу случайными вещественными числами из определенного интервала.
При этом каждый элемент изменяется в своих пределах.
Входные параметры:
 VMHL_ResultMatrix - указатель на матрицу;
 Left - левые границы интервала изменения каждого элемента (размер VMHL_N x VMHL_M);
 Right - правые границы интервала изменения каждого элемента (размер VMHL_N x VMHL_M);
 VMHL_N - размер массива (число строк);
 VMHL_M - размер массива (число столбцов).
Возвращаемое значение:
 Отсутствует.
*/
for (int i=0;i<VMHL_N;i++)
 for (int j=0;j<VMHL_M;j++)
  VMHL_ResultMatrix[i][j]=MHL_RandomUniform(Left[i][j],Right[i][j]);
}

//---------------------------------------------------------------------------
void MHL_RandomRealMatrixInRows(double **VMHL_ResultMatrix, double *Left, double *Right, int VMHL_N, int VMHL_M)
{
/*
Функция заполняет матрицу случайными вещественными числами из определенного интервала.
При этом элементы каждой строки изменяются в своих пределах.
Входные параметры:
 VMHL_ResultMatrix - указатель на матрицу;
 Left - левые границы интервала изменения элементов строки (размер VMHL_N);
 Right - правые границы интервала изменения элементов строки (размер VMHL_N);
 VMHL_N - размер массива (число строк);
 VMHL_M - размер массива (число столбцов).
Возвращаемое значение:
 Отсутствует.
*/
for (int i=0;i<VMHL_N;i++)
 for (int j=0;j<VMHL_M;j++)
  VMHL_ResultMatrix[i][j]=MHL_RandomUniform(Left[i],Right[i]);
}
//---------------------------------------------------------------------------
void MHL_RandomRealVector(double *VMHL_ResultVector, double Left, double Right, int VMHL_N)
{
/*
Функция заполняет массив случайными вещественными числами из определенного интервала [Left;Right].
Входные параметры:
 VMHL_ResultVector - указатель на массив;
 Left - левая граница интервала;
 Right - правая граница интервала;
 VMHL_N - размер массива.
Возвращаемое значение:
 Отсутствует.
*/
for (int i=0;i<VMHL_N;i++)
 VMHL_ResultVector[i]=MHL_RandomUniform(Left,Right);
}
//---------------------------------------------------------------------------
void MHL_RandomRealVectorInElements(double *VMHL_ResultVector, double *Left, double *Right, int VMHL_N)
{
/*
Функция заполняет массив случайными вещественными числами из определенного интервала, где на каждую координату свои границы изменения.
Входные параметры:
 VMHL_ResultVector - указатель на массив;
 Left - левые границы интервалов (размер VMHL_N);
 Right - правые границы интервалов (размер VMHL_N)
 VMHL_N - размер массива.
Возвращаемое значение:
 Отсутствует.
*/
for (int i=0;i<VMHL_N;i++)
 VMHL_ResultVector[i]=MHL_RandomUniform(Left[i],Right[i]);
}
//---------------------------------------------------------------------------
void MHL_RandomVectorOfProbability(double *VMHL_ResultVector, int VMHL_N)
{
/*
Функция заполняет вектор случайными значениями вероятностей. Сумма всех элементов вектора равна 1.
Входные параметры:
 VMHL_ResultVector - указатель на вектор вероятностей (одномерный массив);
 VMHL_N - размер массива.
Возвращаемое значение:
 Отсутствует.
*/
for(int i=0;i<VMHL_N;i++)
 VMHL_ResultVector[i]=MHL_RandomNumber();
//Вычислим сумму вектора
double sum=TMHL_SumVector(VMHL_ResultVector,VMHL_N);
if (sum==0)
 {
 //Если сумма равна нулю
 for (int i=0;i<VMHL_N;i++) VMHL_ResultVector[i]=1./double(VMHL_N);
 }
else
 {
 //Нормализуем
 for (int i=0;i<VMHL_N;i++) VMHL_ResultVector[i]/=sum;
 }
}
//---------------------------------------------------------------------------

//*****************************************************************
//Случайные числа
//*****************************************************************
double MHL_RandomNormal(double Mean, double StdDev)
{
/*
Случайное число по нормальному закону распределения.
Входные параметры:
 Mean - математическое ожидание;
 StdDev - среднеквадратичное отклонение.
Возвращаемое значение:
 Случайное число по нормальному закону.
*/
double VMHL_Result,
s=0.449871,
t=-0.386595,
a=0.19600,
b=0.25472,
r1=0.27597,
r2=0.27846;
double u,v,x,y,q;
bool B;
B=false;
do
 {
 u=MHL_RandomNumber();
 v=MHL_RandomNumber();
 v=1.7156*(v-0.5);
 x=u-s;
 y=fabs(v)-t;
 q=x*x+y*(a*y-b*x);
 if (q<r1)
  B=true;
 else
  if ((q<=r2)&&(v*v<-4.0*log(u)*u*u))
   B=true;
 }
while (B!=true);
if (u==0) return Mean;
VMHL_Result=v/u;
VMHL_Result=VMHL_Result*StdDev+Mean;
return VMHL_Result;
return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_RandomUniform(double a, double b)
{
/*
Случайное вещественное число в интервале [a;b] по равномерному закону распределения.
Входные параметры:
 a - левая граница;
 b - правая граница.
Возвращаемое значение:
 Случайное вещественное число в интервале [a;b].
*/
return (a+MHL_RandomNumber()*(b-a));
}
//---------------------------------------------------------------------------
int MHL_RandomUniformInt(int n, int m)
{
/*
Случайное целое число в интервале [n,m) по равномерному закону распределения.
Входные параметры:
 n - левая граница;
 m - правая граница.
Возвращаемое значение:
 Случайное целое число от n до m-1 включительно.
*/
int VMHL_Result;
VMHL_Result=n+int(MHL_RandomNumber()*(m-n));
if (VMHL_Result==m) VMHL_Result=m-1;
return VMHL_Result;
}
//---------------------------------------------------------------------------
int MHL_RandomUniformIntIncluding(int n, int m)
{
/*
Случайное целое число в интервале [n,m] по равномерному закону распределения.
Входные параметры:
 n - левая граница;
 m - правая граница.
Возвращаемое значение:
 Случайное целое число от n до m включительно.
Примечание:
 В отличии от функции MHL_RandomUniformInt правая граница тоже включается, то есть может сгенерироваться m, а не m-1.
*/
int VMHL_Result;
VMHL_Result=n+int(MHL_RandomNumber()*(m+1-n));
if (VMHL_Result==m+1) VMHL_Result=m+1-1;
return VMHL_Result;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Сортировка
//*****************************************************************

//*****************************************************************
//Статистика и теория вероятности
//*****************************************************************
double MHL_DensityOfDistributionOfNormalDistribution(double x)
{
/*
Плотность распределения вероятности нормированного и центрированного нормального распределения.
Входные параметры:
 x - входная переменная.
Возвращаемое значение:
 Значение функции в точке.
*/
return ((1./sqrt(2.*MHL_PI))*MHL_ExpMSxD2(x));
}
//---------------------------------------------------------------------------
double MHL_DistributionFunctionOfNormalDistribution(double x, double Epsilon)
{
/*
Функция распределения нормированного и центрированного нормального распределения.
Входные параметры:
 x - входная переменная (правая граница интегрирования);
 Epsilon - погрешность (например, Epsilon=0.001).
Возвращаемое значение:
 Значение функции в точке.
*/
return ((1./sqrt(2.*MHL_PI))*MHL_IntegralOfSimpson(0,x,Epsilon,MHL_ExpMSxD2));
}
//---------------------------------------------------------------------------
double MHL_StdDevToVariance(double StdDev)
{
/*
Функция переводит среднеквадратичное уклонение в значение дисперсии случайной величины.
Входные параметры:
 StdDev - среднеквадратичное уклонение.
Возвращаемое значение:
 Значение дисперсии случайной величины.
*/
return sqrt(StdDev);
}
//---------------------------------------------------------------------------
double MHL_VarianceToStdDev(double Variance)
{
/*
Функция переводит значение дисперсии случайной величины в среднеквадратичное уклонение.
Входные параметры:
 Variance - значение дисперсии случайной величины.
Возвращаемое значение:
 Значение среднеквадратичного уклонения.
*/
return Variance*Variance;
}
//---------------------------------------------------------------------------

//*****************************************************************
//Тестовые функции для оптимизации
//*****************************************************************
double MHL_TestFunction_Ackley(double *x, int VMHL_N)
{
/*
Функция многих переменных: Ackley.
Тестовая функция вещественной оптимизации.
Входные параметры:
 x - указатель на исходный массив;
 VMHL_N - размер массива x.
Возвращаемое значение:
 Значение тестовой функции в точке x.
*/
double VMHL_Result;
double f1,f2=0;
f1=exp(-0.2*sqrt(TMHL_SumSquareVector(x,VMHL_N)/double(VMHL_N)));
for (int i=0;i<VMHL_N;i++) f2=f2+cos(2.*MHL_PI*x[i]);
f2=exp(f2/double(VMHL_N));
VMHL_Result=20.+exp(1)-20.*f1-f2;
return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_TestFunction_ParaboloidOfRevolution(double *x, int VMHL_N)
{
/*
Функция многих переменных: Эллиптический параболоид.
Тестовая функция вещественной оптимизации.
Входные параметры:
 x - указатель на исходный массив;
 VMHL_N - размер массива x.
Возвращаемое значение:
 Значение тестовой функции в точке x.
*/
double VMHL_Result=0;
for (int i=0;i<VMHL_N;i++) VMHL_Result+=x[i]*x[i];
return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_TestFunction_Rastrigin(double *x, int VMHL_N)
{
/*
Функция многих переменных: функция Растригина.
Тестовая функция вещественной оптимизации.
Входные параметры:
 x - указатель на исходный массив;
 VMHL_N - размер массива x.
Возвращаемое значение:
 Значение тестовой функции в точке x.
*/
double VMHL_Result=0;
for (int i=0;i<VMHL_N;i++) VMHL_Result+=x[i]*x[i]-10.*cos(2.*MHL_PI*x[i]);
VMHL_Result+=10*VMHL_N;
return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_TestFunction_Rosenbrock(double *x, int VMHL_N)
{
/*
Функция многих переменных: функция Розенброка.
Тестовая функция вещественной оптимизации.
Входные параметры:
 x - указатель на исходный массив;
 VMHL_N - размер массива x.
Возвращаемое значение:
 Значение тестовой функции в точке x.
*/
double VMHL_Result=0;
for (int i=0;i<VMHL_N-1;i++) VMHL_Result+=100.*(x[i+1]-x[i]*x[i])*(x[i+1]-x[i]*x[i])+(1.-x[i])*(1.-x[i]);
return VMHL_Result;
}
//---------------------------------------------------------------------------
double MHL_TestFunction_SumVector(int *x, int VMHL_N)
{
/*
Сумма всех элементов бинарного вектора.
Тестовая функция бинарной оптимизации.
Входные параметры:
 x - указатель на исходный массив;
 VMHL_N - размер массива x.
Возвращаемое значение:
 Значение тестовой функции в точке x.
*/
double VMHL_Result=0;
for (int i=0;i<VMHL_N;i++) VMHL_Result+=x[i];
return VMHL_Result;
}

//---------------------------------------------------------------------------

//*****************************************************************
//Тригонометрические функции
//*****************************************************************
double MHL_Cos(double x)
{
/*
Функция возвращает косинус угла в радианах.
Входные параметры:
 x - угол в радианах.
Возвращаемое значение:
 Косинус угла.
Примечание:
 Вводится только для того, чтобы множество тригонометрических функций было полным.
*/
return cos(x);
}
//---------------------------------------------------------------------------
double MHL_CosDeg(double x)
{
/*
Функция возвращает косинус угла в градусах.
Входные параметры:
 x - угол в градусах.
Возвращаемое значение:
 Косинус угла.
*/
return cos(MHL_DegToRad(x));
}
//---------------------------------------------------------------------------
double MHL_Cosec(double x)
{
/*
Функция возвращает косеканс угла в радианах.
Входные параметры:
 x - угол в радианах.
Возвращаемое значение:
 Косеканс угла.
*/
return 1./sin(x);
}
//---------------------------------------------------------------------------
double MHL_CosecDeg(double x)
{
/*
Функция возвращает косеканс угла в градусах.
Входные параметры:
 x - угол в градусах.
Возвращаемое значение:
 Косеканс угла.
*/
return 1./sin(MHL_DegToRad(x));
}
//---------------------------------------------------------------------------
double MHL_Cotan(double x)
{
/*
Функция возвращает котангенс угла в радианах.
Входные параметры:
 x - угол в радианах.
Возвращаемое значение:
 Котангенс угла.
*/
return 1./tan(x);
}
//---------------------------------------------------------------------------
double MHL_CotanDeg(double x)
{
/*
Функция возвращает котангенс угла в градусах.
Входные параметры:
 x - угол в градусах.
Возвращаемое значение:
 Котангенс угла.
*/
return 1./tan(MHL_DegToRad(x));
}
//---------------------------------------------------------------------------
double MHL_Sec(double x)
{
/*
Функция возвращает секанс угла в радианах.
Входные параметры:
 x - угол в радианах.
Возвращаемое значение:
 Секанс угла.
*/
return 1./cos(x);
}
//---------------------------------------------------------------------------
double MHL_SecDeg(double x)
{
/*
Функция возвращает секанс угла в градусах.
Входные параметры:
 x - угол в градусах.
Возвращаемое значение:
 Секанс угла.
*/
return 1./cos(MHL_DegToRad(x));
}
//---------------------------------------------------------------------------
double MHL_Sin(double x)
{
/*
Функция возвращает синус угла в радианах.
Входные параметры:
 x - угол в радианах.
Возвращаемое значение:
 Синус угла.
Примечание:
 Вводится только для того, чтобы множество тригонометрических функций было полным.
*/
return sin(x);
}
//---------------------------------------------------------------------------
double MHL_SinDeg(double x)
{
/*
Функция возвращает синус угла в градусах.
Входные параметры:
 x - угол в градусах.
Возвращаемое значение:
 Синус угла.
*/
return sin(MHL_DegToRad(x));
}
//---------------------------------------------------------------------------
double MHL_Tan(double x)
{
/*
Функция возвращает тангенс угла в радианах.
Входные параметры:
 x - угол в радианах.
Возвращаемое значение:
 Тангенс угла.
Примечание:
 Вводится только для того, чтобы множество тригонометрических функций было полным.
*/
return tan(x);
}
//---------------------------------------------------------------------------
double MHL_TanDeg(double x)
{
/*
Функция возвращает тангенс угла в градусах.
Входные параметры:
 x - угол в градусах.
Возвращаемое значение:
 Тангенс угла.
*/
return tan(MHL_DegToRad(x));
}
//---------------------------------------------------------------------------

//*****************************************************************
//Уравнения
//*****************************************************************
int MHL_QuadraticEquation(double a, double b, double c, double *x1, double *x2)
{
/*
Функция решает квадратное уравнение вида: a*x*x+b*x+c=0.
Ответ представляет собой два действительных числа.
Входные параметры:
 a - параметр уравнения;
 b - параметр уравнения;
 c - параметр уравнения;
 x1 - первый корень;
 x2 - второй корень.
Возвращаемое значение:
 1 - все хорошо;
 0 - дискриминант отрицательный.
*/
    double D;
    D=b*b-4*a*c;
    if (D>=0)
    {
        *x1=(-b+sqrt(D))/(2.*a);
        *x2=(-b-sqrt(D))/(2.*a);
    }
    else
    {
        *x1=0;
        *x2=0;
        return 0;
    }
    return 1;
}
//---------------------------------------------------------------------------
